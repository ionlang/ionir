#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>
#include <ionir/construct/value.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    void LlvmLoweringPass::visitAllocaInst(ionshared::Ptr<AllocaInst> node) {
        this->requireBuilder();
        node->type->accept(*this);

        llvm::Type* type = this->typeStack.pop();

        /**
         * Create the LLVM-equivalent alloca instruction
         * using the buffered builder.
         */
        llvm::AllocaInst* llvmAllocaInst =
            this->makeLlvmBuilder()->CreateAlloca(type, (llvm::Value*)nullptr);

        this->valueStack.push(llvmAllocaInst);
        this->symbolTable.set(node, llvmAllocaInst);
    }

    void LlvmLoweringPass::visitReturnInst(ionshared::Ptr<ReturnInst> node) {
        this->requireBuilder();

        ionshared::OptPtr<Construct> returnInstValue = node->value;
        llvm::ReturnInst* llvmReturnInst = nullptr;

        if (ionshared::util::hasValue(returnInstValue)) {
            llvm::Value* llvmValue = nullptr;

            // TODO: Hotfix. Clean up messy code.
            // Process the value if applicable.
            if ((*returnInstValue)->constructKind == ConstructKind::Value) {
                returnInstValue->get()->accept(*this);
                llvmValue = this->valueStack.pop();
            }
            // TODO: Ref<> is no longer a construct nor should be used for name resolution (name resolution occurs on Ionlang).
//            // Otherwise, it must be a reference.
//            else if ((*returnInstValue)->getConstructKind() == ConstructKind::Ref) {
//                auto ref = (*returnInstValue)->dynamicCast<Ref<>>();
//
//                if (!ref->isResolved()) {
//                    throw std::runtime_error("Return instruction's value is an unresolved reference");
//                }
//
//                auto key = *ref->getValueAs<Construct>();
//                auto llvmValueFromEntities = this->findInScope(key);
//
//                if (!llvmValueFromEntities.has_value() || *llvmValueFromEntities == nullptr) {
//                    throw std::runtime_error("Could not lookup corresponding LLVM value on the symbol table");
//                }
//
//                llvmValue = *llvmValueFromEntities;
//            }
            else {
                throw std::runtime_error("Unexpected construct as return instruction return value");
            }

            // ------

            /**
             * Create the LLVM equivalent return instruction
             * using the buffered builder.
             */
            llvmReturnInst = this->makeLlvmBuilder()->CreateRet(llvmValue);
        }
        // No value was specified. Simply return void.
        else {
            llvmReturnInst = this->makeLlvmBuilder()->CreateRetVoid();
        }

        this->valueStack.push(llvmReturnInst);
//        this->addToScope(node, llvmReturnInst);
    }

    void LlvmLoweringPass::visitBranchInst(ionshared::Ptr<BranchInst> node) {
        this->requireBuilder();
        this->visit(node->condition);

        llvm::Value *condition = this->valueStack.pop();

        this->lockBuffers([&, this] {
            /**
             * Visit the consequent and alternative basic blocks. There's a
             * mechanism in place to prevent the blocks from being processed
             * twice, wherever they're first encountered. Then try to pop the
             * resulting value because it might not have been emitted if it was
             * processed already.
             */
            this->visitBasicBlock(node->consequentBasicBlock);
            this->valueStack.tryPop();
            this->visitBasicBlock(node->alternativeBasicBlock);
            this->valueStack.tryPop();
        });

        std::optional<llvm::BasicBlock *> llvmConsequentBasicBlock =
            this->symbolTable.find<llvm::BasicBlock>(node->consequentBasicBlock);

        std::optional<llvm::BasicBlock *> llvmAlternativeBasicBlock =
            this->symbolTable.find<llvm::BasicBlock>(node->alternativeBasicBlock);

        if (!ionshared::util::hasValue(llvmConsequentBasicBlock) || !ionshared::util::hasValue(llvmAlternativeBasicBlock)) {
            throw std::runtime_error("Emitted LLVM basic block could not be found");
        }

        // Create the LLVM conditional branch instruction.
        llvm::BranchInst *llvmBranchInst = this->makeLlvmBuilder()->CreateCondBr(
            condition,
            *llvmConsequentBasicBlock,
            *llvmAlternativeBasicBlock
        );

        this->valueStack.push(llvmBranchInst);
//        this->addToScope(node, llvmBranchInst);
    }

    void LlvmLoweringPass::visitCallInst(ionshared::Ptr<CallInst> node) {
        this->requireModule();
        this->requireBuilder();

        ionshared::Ptr<Construct> callee = node->callee;
        ConstructKind calleeConstructKind = callee->constructKind;

        if (calleeConstructKind != ConstructKind::Function && calleeConstructKind != ConstructKind::Extern) {
            // TODO: Use DiagnosticBuilder.
            throw std::runtime_error("Callee is neither a function nor an extern");
        }

        /**
         * If the callee function or extern  has not yet been visited/emitted
         * yet, visit it at this point, as it is required to be present on the
         * emitted entities symbol table in order to emit the call instruction.
         */
        if (!this->symbolTable.contains(callee)) {
            this->lockBuffers([&, this] {
                this->visit(callee);
            });

            // TODO: The function is being discarded here, but what if that function needs to be present in the stack because of the invoking method requires to pop it?
            this->valueStack.tryPop();
        }

        ionshared::Ptr<Prototype> calleePrototype = nullptr;

        if (calleeConstructKind == ConstructKind::Function) {
            calleePrototype = callee->dynamicCast<Function>()->prototype;
        }
        else if (calleeConstructKind == ConstructKind::Extern) {
            calleePrototype = callee->dynamicCast<Extern>()->prototype;
        }
        else {
            throw std::runtime_error("Callee is neither a function nor an extern");
        }

        // Attempt to resolve the callee LLVM-equivalent function.
        llvm::Function* llvmCallee =
            (*this->buffers.llvmModule)->getFunction(calleePrototype->name);

        // LLVM-equivalent function could not be found. Report an error.
        if (llvmCallee == nullptr) {
            throw std::runtime_error("Call instruction referenced an undefined function");
        }

        std::vector<ionshared::Ptr<Construct>> args = node->args;
        std::vector<llvm::Value *> llvmArgs;

        for (const auto &arg : args) {
            this->visit(arg);
            llvmArgs.push_back(this->valueStack.pop());
        }

        // Otherwise, create the LLVM call instruction.
        llvm::CallInst *callInst =
            this->makeLlvmBuilder()->CreateCall(llvmCallee, llvmArgs);

        this->valueStack.push(callInst);
//        this->addToScope(node, callInst);
    }

    void LlvmLoweringPass::visitStoreInst(ionshared::Ptr<StoreInst> node) {
        this->requireFunction();
        this->requireBuilder();

        ionshared::Ptr<AllocaInst> target = node->target;

        std::optional<llvm::AllocaInst*> llvmTargetAlloca =
            this->symbolTable.find<llvm::AllocaInst>(target);

        if (!ionshared::util::hasValue(llvmTargetAlloca)) {
            throw std::runtime_error("Target could not be retrieved from the emitted entities map");
        }

        this->visit(node->value);

        llvm::Value* llvmValue = this->valueStack.pop();

        llvm::StoreInst* llvmStoreInst =
            this->makeLlvmBuilder()->CreateStore(llvmValue, *llvmTargetAlloca);

        this->valueStack.push(llvmStoreInst);
//        this->addToScope(node, llvmStoreInst);
    }

    void LlvmLoweringPass::visitJumpInst(ionshared::Ptr<JumpInst> node) {
        // TODO: Check everything thoroughly. Was just left there uncompleted!
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        this->requireBuilder();

        ionshared::Ptr<BasicBlock> basicBlockTarget = node->basicBlockTarget;

        this->lockBuffers([&, this] {
            /**
             * Visit the basic block. A mechanism will prevent it from being
             * emitted twice. Try popping the resulting value because it might
             * be processed if it has been previously emitted.
             */
            this->visitBasicBlock(basicBlockTarget);
            this->valueStack.tryPop();
        });

        std::optional<llvm::BasicBlock *> llvmBasicBlockResult =
            this->symbolTable.find<llvm::BasicBlock>(basicBlockTarget);

        if (!ionshared::util::hasValue(llvmBasicBlockResult)) {
            // TODO: Throw as internal error (DiagnosticBuilder).
            throw std::runtime_error("Could not find llvm block in emitted entities: " + basicBlockTarget->name);
        }

        // Create the LLVM branch instruction (with no condition).
        llvm::BranchInst *llvmBranchInst =
            this->makeLlvmBuilder()->CreateBr(*llvmBasicBlockResult);

        this->valueStack.push(llvmBranchInst);
//        this->addToScope(node, llvmBranchInst);
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------
        // ------------------------------------------------------------------

    }

    void LlvmLoweringPass::visitOperationInst(ionshared::Ptr<OperationInst> node) {
        llvm::IRBuilder<> llvmBuilder = this->requireBuilder();
        std::optional<llvm::Value*> llvmRightSideValue = std::nullopt;
        bool isInteger = node->leftSideValue->type->typeKind == TypeKind::Integer;

        /**
         * NOTE: During lowering it is assumed that both types are
         * compatible (equal), since there is no implicit conversions
         * done by the compiler. This assurance comes from the type-checking
         * pass, which must occur prior to lowering.
         */
        if (!isInteger && node->leftSideValue->type->typeKind != TypeKind::Decimal) {
            // TODO: Internal error?
            throw std::runtime_error("Value type of operation must be numerical");
        }

        auto requireAndVisitRightSide = [&]{
            if (!ionshared::util::hasValue(node->rightSideValue)) {
                throw std::runtime_error("Right side must have a value for this operation");
            }

            this->visit(*node->rightSideValue);
            llvmRightSideValue = this->valueStack.pop();
        };

        this->visit(node->leftSideValue);

        llvm::Value* llvmLeftSideValue = this->valueStack.pop();
        llvm::Value* result;
        llvm::Instruction::BinaryOps llvmBinaryOperator;
        llvm::Instruction::UnaryOps llvmUnaryOperator;

        switch (node->operatorKind) {
            case OperatorKind::Addition: {
                requireAndVisitRightSide();

                llvmBinaryOperator = isInteger
                    ? llvm::Instruction::BinaryOps::Add
                    : llvm::Instruction::BinaryOps::FAdd;

                break;
            }

            case OperatorKind::Multiplication: {
                requireAndVisitRightSide();

                llvmBinaryOperator = isInteger
                    ? llvm::Instruction::BinaryOps::Mul
                    : llvm::Instruction::BinaryOps::FMul;

                break;
            }

            case OperatorKind::Subtraction: {
                if (ionshared::util::hasValue(node->rightSideValue)) {
                    llvmBinaryOperator = isInteger
                        ? llvm::Instruction::BinaryOps::Sub
                        : llvm::Instruction::BinaryOps::FSub;

                    break;
                }

                // TODO: Implement lowering of integer neg. operator (via 0 - value?).
                llvmUnaryOperator = isInteger
                    ? throw std::runtime_error("Not yet implemented")
                    : llvm::Instruction::UnaryOps::FNeg;

                break;
            }

            case OperatorKind::And: {
                llvmBinaryOperator = llvm::Instruction::BinaryOps::And;

                break;
            }

            case OperatorKind::Or: {
                llvmBinaryOperator = llvm::Instruction::BinaryOps::Or;

                break;
            }

            // TODO: Add support for missing operators.

            default: {
                throw std::runtime_error("Unsupported operator kind");
            }
        }

        if (ionshared::util::hasValue(node->rightSideValue)) {
            result = llvmBuilder.CreateBinOp(
                llvmBinaryOperator,
                llvmLeftSideValue,
                *llvmRightSideValue
            );
        }
        else {
            result = llvmBuilder.CreateUnOp(
                llvmUnaryOperator,
                llvmLeftSideValue
            );
        }

        this->valueStack.push(result);
    }
}
