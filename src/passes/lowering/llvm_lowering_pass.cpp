#include <iostream>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    llvm::IRBuilder<> LlvmLoweringPass::LlvmBuffers::makeBuilder() {
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

    std::shared_ptr<ionshared::SymbolTable<llvm::Module*>> LlvmLoweringPass::getModules() const {
        return this->modules;
    }

    LlvmLoweringPass::LlvmLoweringPass(
        std::shared_ptr<ionshared::PassContext> context,
        std::shared_ptr<ionshared::SymbolTable<llvm::Module*>> modules
    ) noexcept :
        Pass(std::move(context)),
        modules(std::move(modules)),
        llvmBuffers(),
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

        std::vector<std::shared_ptr<Instruction>> instructions = construct->instructions;

        // Emit the entity at this point so visiting children can access it.
        this->valueSymbolTable.set(construct, llvmBasicBlock);

        for (const auto& instruction : instructions) {
            this->visit(instruction);
        }

        this->llvmBuffers.basicBlocks.forcePop();
    }

    void LlvmLoweringPass::visitFunctionBody(std::shared_ptr<FunctionBody> node) {
        /**
         * Entry section must be set. Redundant check, since the verify should
         * function ensure that the block contains a single entry section, but
         * just to make sure.
         */
        if (node->basicBlocks.empty()) {
            throw std::runtime_error("No entry basic block exists for block");
        }

        ionshared::OptPtr<BasicBlock> entryBasicBlock{node->basicBlocks.begin()->get()};

        TypeKind parentFunctionPrototypeReturnKind =
            node->forceGetUnboxedParent()->prototype->returnType->typeKind;

        /**
         * The function body's entry basic block contains no terminal instruction.
         * The parent function's prototype's return type is void. Implicitly append
         * a return void instruction to satisfy LLVM's terminal instruction requirement.
         */
        if (!entryBasicBlock->get()->hasTerminalInst() && parentFunctionPrototypeReturnKind == TypeKind::Void) {
            entryBasicBlock->get()->appendInst(std::make_shared<ReturnInst>(
                *entryBasicBlock
            ));
        }

        for (const auto& basicBlock : node->basicBlocks) {
            this->visit(basicBlock);
        }
    }

    void LlvmLoweringPass::visitGlobal(std::shared_ptr<Global> construct) {
        llvm::Type* llvmType = this->eagerVisitType(construct->type);

        auto* llvmGlobalVariable = llvm::dyn_cast<llvm::GlobalVariable>(
            this->llvmBuffers.modules.forceGetTopItem()
                ->getOrInsertGlobal(construct->name, llvmType)
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

    void LlvmLoweringPass::visitIntegerType(std::shared_ptr<IntegerType> construct) {
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

    void LlvmLoweringPass::visitBooleanType(std::shared_ptr<BooleanType> construct) {
        this->typeSymbolTable.set(construct, this->processTypeQualifiers(
            construct->qualifiers,

            llvm::Type::getInt1Ty(
                this->llvmBuffers.modules.forceGetTopItem()->getContext()
            )
        ));
    }

    void LlvmLoweringPass::visitVoidType(std::shared_ptr<VoidType> construct) {
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
            **construct->identifier,
            *new llvm::LLVMContext()
        );

        this->llvmBuffers.modules.push(llvmModule);
        this->moduleSymbolTable.set(construct, llvmModule);
        this->modules->set(**construct->identifier, llvmModule.get());

        // Proceed to visit all the module's children (top-level constructs).
        std::map<std::string, std::shared_ptr<Construct>> moduleNativeSymbolTable =
            construct->context->getGlobalScope()->unwrap();

        for (const auto& [id, topLevelConstruct] : moduleNativeSymbolTable) {
            this->visit(topLevelConstruct);
        }

        this->llvmBuffers.modules.forcePop();
    }

    void LlvmLoweringPass::visitStruct(std::shared_ptr<Struct> construct) {
        auto fieldsNativeMap = construct->fields->unwrap();
        std::vector<llvm::Type*> llvmFields{};

        llvmFields.reserve(fieldsNativeMap.size());

        for (const auto& [name, type] : fieldsNativeMap) {
            llvmFields.push_back(this->eagerVisitType(type));
        }

        llvm::StructType* llvmStruct = llvm::StructType::get(
            this->llvmBuffers.modules.forceGetTopItem()->getContext(),
            llvmFields
        );

        llvmStruct->setName(construct->name);
        this->typeSymbolTable.set(construct, llvmStruct);
    }

    void LlvmLoweringPass::visitStructDefinition(std::shared_ptr<StructDefinition> construct) {
        std::vector<llvm::Value*> llvmValues{};

        for (const auto& value : construct->values) {
            llvmValues.push_back(this->eagerVisitValue(value));
        }

        llvm::IRBuilder<> llvmBuilder = this->llvmBuffers.makeBuilder();

        llvm::AllocaInst* structAllocation = llvmBuilder.CreateAlloca(
            this->eagerVisitType(
                construct->type
            )
        );

        for (size_t i = 0; i < llvmValues.size(); i++) {
            llvm::Value* structElement = llvmBuilder.CreateStructGEP(structAllocation, i);

            llvmBuilder.CreateStore(llvmValues[i], structElement);
        }

        this->valueSymbolTable.set(construct, structAllocation);
    }
}
