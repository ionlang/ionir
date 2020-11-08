#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>
#include <ionir/construct/value.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    void LlvmLoweringPass::visitAllocaInst(std::shared_ptr<AllocaInst> construct) {
        // TODO: Option to create array here too?
        /**
         * Create the LLVM-equivalent alloca instruction
         * using the buffered builder.
         */
        this->valueSymbolTable.set(
            construct,

            this->llvmBuffers.makeBuilder().CreateAlloca(
                this->eagerVisitType(construct->type),
                (llvm::Value*)nullptr
            )
        );
    }

    void LlvmLoweringPass::visitReturnInst(std::shared_ptr<ReturnInst> construct) {
        llvm::ReturnInst* llvmReturnInst = ionshared::util::hasValue(construct->value)
            ? this->llvmBuffers.makeBuilder()
                .CreateRet(this->eagerVisitValue(*construct->value))

            : this->llvmBuffers.makeBuilder().CreateRetVoid();

        this->valueSymbolTable.set(construct, llvmReturnInst);
    }

    void LlvmLoweringPass::visitBranchInst(std::shared_ptr<BranchInst> construct) {
        this->valueSymbolTable.set(construct, this->llvmBuffers.makeBuilder().CreateCondBr(
            this->eagerVisitValue(construct->condition),

            this->eagerVisitValue<llvm::BasicBlock>(
                construct->consequentBasicBlock
            ),

            this->eagerVisitValue<llvm::BasicBlock>(
                construct->alternativeBasicBlock
            )
        ));
    }

    void LlvmLoweringPass::visitCallInst(std::shared_ptr<CallInst> construct) {
        ConstructKind calleeConstructKind = construct->callee->constructKind;

        if (calleeConstructKind != ConstructKind::Function && calleeConstructKind != ConstructKind::Extern) {
            // TODO: Use DiagnosticBuilder: internal error.
            throw std::runtime_error("Callee is neither a function nor an extern");
        }

        std::shared_ptr<Prototype> calleePrototype =
            calleeConstructKind == ConstructKind::Function
                ? construct->callee->dynamicCast<Function>()->prototype
                : construct->callee->dynamicCast<Extern>()->prototype;

        auto* llvmCallee = this->eagerVisitValue<llvm::Function>(construct->callee);

        // LLVM-equivalent function could not be found. Report an error.
        if (llvmCallee == nullptr) {
            throw std::runtime_error("Call instruction referenced an undefined function");
        }

        std::vector<std::shared_ptr<Construct>> args = construct->arguments;
        std::vector<llvm::Value*> llvmArgs{};

        llvmArgs.reserve(args.size());

        for (const auto& arg : args) {
            llvmArgs.push_back(this->eagerVisitValue(arg));
        }

        this->valueSymbolTable.set(
            construct,
            this->llvmBuffers.makeBuilder().CreateCall(llvmCallee, llvmArgs)
        );
    }

    void LlvmLoweringPass::visitStoreInst(std::shared_ptr<StoreInst> construct) {
        this->valueSymbolTable.set(construct, this->llvmBuffers.makeBuilder().CreateStore(
            this->eagerVisitValue(construct->value),
            this->eagerVisitValue(construct->target)
        ));
    }

    void LlvmLoweringPass::visitJumpInst(std::shared_ptr<JumpInst> node) {
        this->valueSymbolTable.set(
            node,

            this->llvmBuffers.makeBuilder().CreateBr(
                this->eagerVisitValue<llvm::BasicBlock>(node->basicBlockTarget)
            )
        );
    }

    void LlvmLoweringPass::visitCastInst(std::shared_ptr<CastInst> construct) {
        // TODO: Hard-coded here.
        std::vector<TypeKind> castableTypeKinds{
            TypeKind::Integer,
            TypeKind::Decimal,
            TypeKind::Pointer
        };

        bool isCastTypeCastable =
            ionshared::util::vectorContains(castableTypeKinds, construct->type->typeKind);

        bool isValueTypeCastable =
            ionshared::util::vectorContains(castableTypeKinds, construct->value->type->typeKind);

        if (!isCastTypeCastable || !isValueTypeCastable) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::castTypeInvalid)

                // TODO: Passing in optional source location. This might be OK, but need verification.
                ->setSourceLocation(construct->sourceLocation)

                ->formatMessage(
                    isCastTypeCastable
                        ? construct->value->type->typeName
                        : construct->type->typeName
                )

                ->finish();

            // TODO: Should return instead, but what about the other visitors?
            throw std::runtime_error("Awaiting diagnostic API implementation");
        }

        llvm::Instruction::CastOps llvmCastOperation;
        bool isSameTypeKind = construct->type->typeKind == construct->value->type->typeKind;

        if (isSameTypeKind) {
            bool isDowncast;

            switch (construct->type->typeKind) {
                case TypeKind::Integer: {
                    std::shared_ptr<IntegerType> castIntegerType =
                        construct->type->dynamicCast<IntegerType>();

                    std::shared_ptr<IntegerType> valueIntegerType =
                        construct->value->type->dynamicCast<IntegerType>();

                    isDowncast = castIntegerType->integerKind < valueIntegerType->integerKind;

                    break;
                }

                case TypeKind::Decimal: {
                    std::shared_ptr<DecimalType> castDecimalType =
                        construct->type->dynamicCast<DecimalType>();

                    std::shared_ptr<DecimalType> valueDecimalType =
                        construct->value->type->dynamicCast<DecimalType>();

                    isDowncast = valueDecimalType->decimalKind < valueDecimalType->decimalKind;

                    break;
                }

                default: {
                    // TODO: Use diagnostics API (error).
                    throw std::runtime_error("Only integer and decimal types can perform upcast/downcast");
                }
            }

            llvmCastOperation = isDowncast
                ? llvm::Instruction::CastOps::Trunc
                : llvm::Instruction::CastOps::ZExt;
        }
        else {
            switch (construct->type->typeKind) {
                case TypeKind::Integer: {
                    llvmCastOperation = construct->type->dynamicCast<IntegerType>()->isSigned
                        ? llvm::Instruction::CastOps::FPToSI
                        : llvm::Instruction::CastOps::FPToUI;

                    break;
                }

                case TypeKind::Decimal: {
                    llvmCastOperation = construct->type->dynamicCast<DecimalType>()->isSigned
                        ? llvm::Instruction::CastOps::SIToFP
                        : llvm::Instruction::CastOps::UIToFP;

                    break;
                }

                case TypeKind::Pointer: {
                    // TODO
                    throw std::runtime_error("Not implemented");
                }

                default: {
                    // TODO
                    throw std::runtime_error("Not implemented");
                }
            }
        }

        this->valueSymbolTable.set(
            construct,

            this->llvmBuffers.makeBuilder().CreateCast(
                llvmCastOperation,
                this->eagerVisitValue(construct->value),
                this->eagerVisitType(construct->type)
            )
        );
    }
}
