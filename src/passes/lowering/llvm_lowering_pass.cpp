#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

#include <iostream>

namespace ionir {
    void LlvmLoweringPass::Buffers::requireContext() const {
        if (!ionshared::util::hasValue(this->llvmContext)) {
            throw std::runtime_error("Expected the context buffer to be set, but was null");
        }
    }

    void LlvmLoweringPass::Buffers::requireModule() const {
        if (!ionshared::util::hasValue(this->llvmModule)) {
            throw std::runtime_error("Expected the module buffer to be set, but was null");
        }
    }

    void LlvmLoweringPass::Buffers::requireFunction() const {
        if (!ionshared::util::hasValue(this->llvmFunction)) {
            throw std::runtime_error("Expected the function buffer to be set, but was null");
        }
    }

    llvm::IRBuilder<> LlvmLoweringPass::Buffers::requireBuilder() {
        // Builder must be instantiated.
        if (!this->llvmBasicBlock.has_value()) {
            // Otherwise, throw a runtime error.
            throw std::runtime_error("Expected builder to be instantiated");
        }

        return llvm::IRBuilder<>(this->llvmBasicBlock->get());
    }

    std::shared_ptr<llvm::Type> LlvmLoweringPass::processTypeQualifiers(
        const std::shared_ptr<TypeQualifiers>& qualifiers,
        llvm::Type* type
    ) {
        // TODO: This should be last? Or const?
        if (qualifiers->contains(TypeQualifier::Pointer)) {
            // TODO: Address space correct?
            type = llvm::PointerType::get(type, 0);
        }

        // TODO: Process other qualifiers.

        return std::shared_ptr<llvm::Type>(type);
    }

    std::shared_ptr<ionshared::SymbolTable<llvm::Module*>> LlvmLoweringPass::getModules() const {
        return this->modules;
    }

    std::optional<llvm::Module*> LlvmLoweringPass::getModuleBuffer() const {
        return this->buffers.llvmModule;
    }

    bool LlvmLoweringPass::setModuleBuffer(const std::string& id) {
        if (this->modules->contains(id)) {
            this->buffers.llvmModule = this->modules->lookup(id);
            this->buffers.llvmContext = &(*this->buffers.llvmModule)->getContext();

            return true;
        }

        return false;
    }

    LlvmLoweringPass::LlvmLoweringPass(
        std::shared_ptr<ionshared::PassContext> context,
        std::shared_ptr<ionshared::SymbolTable<llvm::Module*>> modules
    ) noexcept :
        Pass(std::move(context)),
        modules(std::move(modules)),
        buffersStack(),
        valueSymbolTable(),
        typeSymbolTable() {
        //
    }

    LlvmLoweringPass::~LlvmLoweringPass() {
        // TODO?
    }

    void LlvmLoweringPass::visitScopeAnchor(std::shared_ptr<ScopeAnchor<>> node) {
        Pass::visitScopeAnchor(node);

        /**
         * Append the scope of the visited scope anchor construct.
         * Before the visit method of the construct exits, it itself
         * must pop the lastly added scope from the context.
         */
        this->buffers.context->appendScope(node->getSymbolTable());
    }

    void LlvmLoweringPass::visitBasicBlock(std::shared_ptr<BasicBlock> construct) {
        // TODO: This is a temporary fix? The other option is to place the block beforehand and emit it, but isn't it the same thing? Investigate.

        std::shared_ptr<Buffers> buffers = this->buffersStack.forceGetTopItem();

        buffers->requireContext();
        buffers->requireFunction();

        /**
         * Create the basic block and at the same time register it
         * under the buffer function.
         */
        std::shared_ptr<llvm::BasicBlock> llvmBasicBlock = std::shared_ptr<llvm::BasicBlock>(
            llvm::BasicBlock::Create(
                **buffers->llvmContext,
                construct->name,
                buffers->llvmFunction->get()
            )
        );

        buffers->llvmBasicBlock = llvmBasicBlock;

        std::vector<std::shared_ptr<Instruction>> instructions = construct->instructions;

        // Emit the entity at this point so visiting children can access it.
        this->valueSymbolTable.set(construct, llvmBasicBlock);

        for (const auto& instruction : instructions) {
            this->visit(instruction);
        }
    }

    void LlvmLoweringPass::visitFunctionBody(std::shared_ptr<FunctionBody> node) {
        /**
         * Retrieve the entry section from the block. At this point, it
         * should be guaranteed to be set.
         */
        ionshared::OptPtr<BasicBlock> entryBasicBlock = node->findEntryBasicBlock();

        /**
         * Entry section must be set. Redundant check, since the verify should
         * function ensure that the block contains a single entry section, but
         * just to make sure.
         */
        if (!ionshared::util::hasValue(entryBasicBlock)) {
            throw std::runtime_error("No entry basic block exists for block");
        }

        TypeKind parentFunctionPrototypeReturnKind =
            node->getUnboxedParent()->prototype->returnType->typeKind;

        /**
         * The function body's entry basic block contains no terminal instruction.
         * The parent function's prototype's return type is void. Implicitly append
         * a return void instruction to satisfy LLVM's terminal instruction requirement.
         */
        if (!entryBasicBlock->get()->hasTerminalInst() && parentFunctionPrototypeReturnKind == TypeKind::Void) {
            entryBasicBlock->get()->appendInst(std::make_shared<ReturnInst>(ReturnInstOpts{
                *entryBasicBlock
            }));
        }

        // Visit all the block's section(s).
        for (const auto& [key, basicBlock] : node->getSymbolTable()->unwrap()) {
            this->visit(basicBlock);
        }

        this->buffers.context->popScope();
    }

    void LlvmLoweringPass::visitGlobal(std::shared_ptr<Global> construct) {
        this->requireModule();

        std::shared_ptr<llvm::Type> llvmType =
            this->typeSafeEarlyVisitOrLookup(construct->type);

        std::shared_ptr<llvm::GlobalVariable> llvmGlobalVariable =
            std::shared_ptr<llvm::GlobalVariable>(llvm::dyn_cast<llvm::GlobalVariable>(
                (*this->buffers.llvmModule)->getOrInsertGlobal(construct->name, llvmType.get())
            ));

        // Initialize the global variable if applicable.
        if (ionshared::util::hasValue(construct->value)) {
            std::shared_ptr<llvm::Value> llvmValue =
                this->valueSafeEarlyVisitOrLookup(*construct->value);

            // TODO: Value needs to be created from below commented statement.
            // llvm::Constant* initializerValue = llvm::Constant::getIntegerValue(llvm::Type);

            // TODO: CRITICAL: You can't just cast llvm::value to constant! See above.
            llvmGlobalVariable->setInitializer(llvm::dyn_cast<llvm::Constant>(llvmValue.get()));
        }

        this->valueSymbolTable.set(construct, llvmGlobalVariable);

        // TODO: Apply LLVM entity to the node.
    }

    void LlvmLoweringPass::visitIntegerType(std::shared_ptr<IntegerType> construct) {
        this->requireContext();

        std::optional<llvm::IntegerType*> type = std::nullopt;

        /**
         * Create the corresponding LLVM integer type based off the
         * node's integer kind.
         */
        switch (construct->integerKind) {
            case IntegerKind::Int8: {
                type = llvm::Type::getInt8Ty(**this->buffers.llvmContext);

                break;
            }

            case IntegerKind::Int16: {
                type = llvm::Type::getInt16Ty(**this->buffers.llvmContext);

                break;
            }

            case IntegerKind::Int32: {
                type = llvm::Type::getInt32Ty(**this->buffers.llvmContext);

                break;
            }

            case IntegerKind::Int64: {
                type = llvm::Type::getInt64Ty(**this->buffers.llvmContext);

                break;
            }

            case IntegerKind::Int128: {
                type = llvm::Type::getInt128Ty(**this->buffers.llvmContext);

                break;
            }

            default: {
                throw std::runtime_error("An unrecognized integer kind was provided");
            }
        }

        // At this point, type must be defined.
        if (!type.has_value()) {
            throw std::runtime_error("Expected type to be defined");
        }

        this->typeSymbolTable.set(construct, this->processTypeQualifiers(
            construct->qualifiers,
            *type
        ));
    }

    void LlvmLoweringPass::visitBooleanType(std::shared_ptr<BooleanType> construct) {
        this->requireContext();

        this->typeSymbolTable.set(construct, this->processTypeQualifiers(
            construct->qualifiers,
            llvm::Type::getInt1Ty(**this->buffers.llvmContext)
        ));
    }

    void LlvmLoweringPass::visitVoidType(std::shared_ptr<VoidType> construct) {
        this->requireContext();

        this->typeSymbolTable.set(construct, this->processTypeQualifiers(
            construct->qualifiers,
            llvm::Type::getVoidTy(**this->buffers.llvmContext)
        ));
    }

    void LlvmLoweringPass::visitModule(std::shared_ptr<Module> node) {
        std::shared_ptr<Buffers> newBuffers = std::make_shared<Buffers>(Buffers{
            .llvmContext = new llvm::LLVMContext(),

            .llvmModule = new llvm::Module(
                **node->identifier,
                **newBuffers->llvmContext
            )
        });

        this->buffersStack.push(newBuffers);
        this->modules->set(**node->identifier, *newBuffers->llvmModule);

        // Proceed to visit all the module's children (top-level constructs).
        std::map<std::string, std::shared_ptr<Construct>> moduleNativeSymbolTable =
            node->context->getGlobalScope()->unwrap();

        for (const auto& [id, topLevelConstruct] : moduleNativeSymbolTable) {
            this->visit(topLevelConstruct);
        }

        this->buffersStack.forcePop();
    }

    void LlvmLoweringPass::visitStruct(std::shared_ptr<Struct> construct) {
        this->requireModule();
        this->requireContext();

        auto fieldsNativeMap = construct->fields->unwrap();
        std::vector<llvm::Type*> llvmFields{};

        llvmFields.reserve(fieldsNativeMap.size());

        for (const auto& [name, type] : fieldsNativeMap) {
            llvmFields.push_back(this->typeSafeEarlyVisitOrLookup(type).get());
        }

        // TODO: Ensure struct isn't already defined, and doesn't exist on the LLVM module (or locally?).

        std::shared_ptr<llvm::StructType> llvmStruct =
            std::shared_ptr<llvm::StructType>(llvm::StructType::get(
                **this->buffers.llvmContext,
                llvmFields
            ));

        llvmStruct->setName(construct->name);
        this->typeSymbolTable.set(construct, llvmStruct);
    }

    void LlvmLoweringPass::visitStructDefinition(std::shared_ptr<StructDefinition> construct) {
        std::vector<llvm::Value*> llvmValues{};

        for (const auto& value : construct->values) {
            llvmValues.push_back(this->valueSafeEarlyVisitOrLookup(value).get());
        }

        llvm::IRBuilder<> builder = this->requireBuilder();

        std::shared_ptr<llvm::AllocaInst> structAllocation =
            std::shared_ptr<llvm::AllocaInst>(builder.CreateAlloca(
                this->typeSafeEarlyVisitOrLookup(
                    construct->declaration
                ).get()
            ));

        for (size_t i = 0; i < llvmValues.size(); i++) {
            llvm::Value* structElement = builder.CreateStructGEP(structAllocation.get(), i);

            builder.CreateStore(llvmValues[i], structElement);
        }

        this->valueSymbolTable.set(construct, structAllocation);
    }
}
