#include <iostream>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Verifier.h>
#include <ionir/tracking/name_mangler.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    llvm::IRBuilder<> LlvmBuffers::makeBuilder() {
        return llvm::IRBuilder<>(this->basicBlocks.forceGetTopItem());
    }

    llvm::Type* LlvmLoweringPass::processTypeQualifiers(
        const std::shared_ptr<TypeQualifiers>& qualifiers,
        llvm::Type* llvmType
    ) {
        llvm::Type* result = llvmType;

        // TODO: This should be last? Or const?
        if (qualifiers->contains(TypeQualifier::Pointer)) {
            // TODO: Address space correct?
            result = llvm::PointerType::get(llvmType, 0);
        }

        // TODO: Process other qualifiers.

        return result;
    }

    LlvmLoweringPass::LlvmLoweringPass(
        std::shared_ptr<ionshared::PassContext> context
    ) noexcept :
        Pass(std::move(context)),
        llvmModules(ionshared::util::makePtrSymbolTable<llvm::Module>()),
        llvmBuffers(),
        localBuffers(),
        moduleSymbolTable(),
        valueSymbolTable(),
        typeSymbolTable() {
        //
    }

    void LlvmLoweringPass::visitScopeAnchor(std::shared_ptr<ScopeAnchor<>> node) {
        Pass::visitScopeAnchor(node);
    }

    void LlvmLoweringPass::visitBasicBlock(std::shared_ptr<BasicBlock> construct) {
        // TODO: This is a temporary fix? The other option is to place the block beforehand and emit it, but isn't it the same thing? Investigate.

        /**
         * Create the basic block and at the same time register it
         * under the buffer function.
         */
        llvm::BasicBlock* llvmBasicBlock = llvm::BasicBlock::Create(
            this->llvmBuffers.modules.forceGetTopItem()->getContext(),

            // TODO: Does LLVM require 'entry' name for the entry basic block?
            "",
            this->llvmBuffers.functions.forceGetTopItem()
        );

        this->llvmBuffers.basicBlocks.push(llvmBasicBlock);

        std::vector<std::shared_ptr<Instruction>> instructions =
            construct->getInstructions();

        // Emit the entity at this point so visiting children can access it.
        this->valueSymbolTable.set(construct, llvmBasicBlock);

        for (const auto& instruction : instructions) {
            this->visit(instruction);
        }

        this->llvmBuffers.basicBlocks.forcePop();
    }

    void LlvmLoweringPass::visitGlobal(std::shared_ptr<Global> construct) {
        llvm::Type* llvmType = this->eagerVisitType(construct->type);

        auto* llvmGlobalVariable = llvm::dyn_cast<llvm::GlobalVariable>(
            this->llvmBuffers.modules.forceGetTopItem()
                ->getOrInsertGlobal(
                    NameMangler::mangle(
                        this->localBuffers.modules.forceGetTopItem(),
                        construct->name
                    ),

                    llvmType
                )
        );

        // Initialize the global variable if applicable.
        if (ionshared::util::hasValue(construct->value)) {
            llvm::Value* llvmValue = this->eagerVisitValue(*construct->value);

            // TODO: Value needs to be created from below commented statement.
            // llvm::Constant* initializerValue = llvm::Constant::getIntegerValue(llvm::Type);

            // TODO: CRITICAL: You can't just cast llvm::value to constant! See above.
            llvmGlobalVariable->setInitializer(llvm::dyn_cast<llvm::Constant>(llvmValue));
        }

        this->valueSymbolTable.set(construct, llvmGlobalVariable);

        // TODO: Apply LLVM entity to the node.
    }

    void LlvmLoweringPass::visitIntegerType(std::shared_ptr<TypeInteger> construct) {
        std::optional<llvm::IntegerType*> llvmType{std::nullopt};

        llvm::LLVMContext& llvmContext =
            this->llvmBuffers.modules.forceGetTopItem()->getContext();

        /**
         * Create the corresponding LLVM integer type based off the
         * node's integer kind.
         */
        switch (construct->integerKind) {
            case IntegerKind::Int8: {
                llvmType = llvm::Type::getInt8Ty(llvmContext);

                break;
            }

            case IntegerKind::Int16: {
                llvmType = llvm::Type::getInt16Ty(llvmContext);

                break;
            }

            case IntegerKind::Int32: {
                llvmType = llvm::Type::getInt32Ty(llvmContext);

                break;
            }

            case IntegerKind::Int64: {
                llvmType = llvm::Type::getInt64Ty(llvmContext);

                break;
            }

            case IntegerKind::Int128: {
                llvmType = llvm::Type::getInt128Ty(llvmContext);

                break;
            }

            default: {
                // TODO: Use diagnostics API.
                throw std::runtime_error("An unrecognized integer kind was provided");
            }
        }

        // At this point, type must be defined.
        if (!llvmType.has_value()) {
            // TODO: Use diagnostics API.
            throw std::runtime_error("Expected type to be defined");
        }

        this->typeSymbolTable.set(construct, this->processTypeQualifiers(
            construct->qualifiers,
            *llvmType
        ));
    }

    void LlvmLoweringPass::visitBooleanType(std::shared_ptr<TypeBoolean> construct) {
        this->typeSymbolTable.set(construct, this->processTypeQualifiers(
            construct->qualifiers,

            llvm::Type::getInt1Ty(
                this->llvmBuffers.modules.forceGetTopItem()->getContext()
            )
        ));
    }

    void LlvmLoweringPass::visitVoidType(std::shared_ptr<TypeVoid> construct) {
        this->typeSymbolTable.set(construct, this->processTypeQualifiers(
            construct->qualifiers,

            llvm::Type::getVoidTy(
                this->llvmBuffers.modules.forceGetTopItem()->getContext()
            )
        ));
    }

    void LlvmLoweringPass::visitModule(std::shared_ptr<Module> construct) {
        // TODO: Should the context be manually freed? Remember, whoever allocates must free.
        std::shared_ptr<llvm::Module> llvmModule = std::make_shared<llvm::Module>(
            NameMangler::mangle(**construct->identifier),
            *new llvm::LLVMContext()
        );

        this->llvmBuffers.modules.push(llvmModule);
        this->localBuffers.modules.push(construct);
        this->moduleSymbolTable.set(construct, llvmModule);
        this->llvmModules->set(**construct->identifier, llvmModule);

        // Proceed to visit all the module's children (top-level constructs).
        std::map<std::string, std::shared_ptr<Construct>> moduleNativeSymbolTable =
            construct->context->globalScope->unwrap();

        for (const auto& [id, topLevelConstruct] : moduleNativeSymbolTable) {
            this->visit(topLevelConstruct);
        }

        IONIR_PASS_INTERNAL_ASSERT(
            llvm::verifyModule(
                *this->llvmBuffers.modules.forceGetTopItem()
            )
        )

        this->llvmBuffers.modules.forcePop();
        this->localBuffers.modules.forcePop();
    }

    void LlvmLoweringPass::visitStructType(std::shared_ptr<TypeStruct> construct) {
        auto fieldsNativeMap = construct->fields->unwrap();
        std::vector<llvm::Type*> llvmFields{};

        llvmFields.reserve(fieldsNativeMap.size());

        for (const auto& [name, type] : fieldsNativeMap) {
            llvmFields.push_back(this->eagerVisitType(type));
        }

        this->typeSymbolTable.set(construct, llvm::StructType::create(
            this->llvmBuffers.modules.forceGetTopItem()->getContext(),
            llvmFields,

            NameMangler::mangle(
                this->localBuffers.modules.forceGetTopItem(),
                construct->typeName
            )
        ));
    }

    void LlvmLoweringPass::visitStructDefinition(std::shared_ptr<StructDefinition> construct) {
        std::vector<llvm::Value*> llvmValues{};

        for (const auto& value : construct->values) {
            llvmValues.push_back(this->eagerVisitValue(value));
        }

        llvm::IRBuilder<> llvmBuilder = this->llvmBuffers.makeBuilder();

        llvm::AllocaInst* llvmAllocaInst = llvmBuilder.CreateAlloca(
            this->eagerVisitType<llvm::StructType>(construct->type)
        );

        for (size_t i = 0; i < llvmValues.size(); i++) {
            llvmBuilder.CreateStore(
                llvmValues[i],
                llvmBuilder.CreateStructGEP(llvmAllocaInst, i)
            );
        }

        this->valueSymbolTable.set(construct, llvmAllocaInst);
    }

    void LlvmLoweringPass::visitNameOfIntrinsic(std::shared_ptr<NameOfIntrinsic> construct) {
        this->valueSymbolTable.set(
            construct,

            this->llvmBuffers.makeBuilder()
                .CreateGlobalStringPtr(construct->type->typeName)
        );
    }

    void LlvmLoweringPass::visitTypeOfIntrinsic(std::shared_ptr<TypeOfIntrinsic> construct) {
        this->typeSymbolTable.set(
            construct,
            this->eagerVisitType(construct->value->type)
        );
    }
}
