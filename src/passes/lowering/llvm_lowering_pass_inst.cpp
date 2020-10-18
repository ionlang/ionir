#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>
#include <ionir/construct/value.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    void LlvmLoweringPass::visitAllocaInst(std::shared_ptr<AllocaInst> construct) {
        this->requireBuilder();

        // TODO: Option to create array here too?
        /**
         * Create the LLVM-equivalent alloca instruction
         * using the buffered builder.
         */
        this->valueSymbolTable.set(
            construct,

            std::shared_ptr<llvm::Value>(this->makeLlvmBuilder()->CreateAlloca(
                this->typeSafeEarlyVisitOrLookup(construct->type).get(),
                (llvm::Value*)nullptr
            ))
        );
    }

    void LlvmLoweringPass::visitReturnInst(std::shared_ptr<ReturnInst> node) {
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

    void LlvmLoweringPass::visitBranchInst(std::shared_ptr<BranchInst> node) {
        this->requireBuilder();

        std::shared_ptr<llvm::Value> llvmCondition =
            this->valueSafeEarlyVisitOrLookup(node->condition);

        std::shared_ptr<llvm::BasicBlock> llvmConsequentBasicBlock;
        std::shared_ptr<llvm::BasicBlock> llvmAlternativeBasicBlock;

        /**
         * Visit the consequent and alternative basic blocks. There's a
         * mechanism in place to prevent the blocks from being processed
         * twice, wherever they're first encountered. Then try to pop the
         * resulting value because it might not have been emitted if it was
         * processed already.
         */
        this->stashBuffers([&, this] {
            llvmConsequentBasicBlock =
                this->valueSafeEarlyVisitOrLookup<llvm::BasicBlock>(node->consequentBasicBlock);

            llvmAlternativeBasicBlock =
                this->valueSafeEarlyVisitOrLookup<llvm::BasicBlock>(node->alternativeBasicBlock);
        });

        this->valueSymbolTable.set(node, std::shared_ptr<llvm::Value>(
            this->makeLlvmBuilder()->CreateCondBr(
                llvmCondition.get(),
                llvmConsequentBasicBlock.get(),
                llvmAlternativeBasicBlock.get()
            ))
        );

//        this->addToScope(node, llvmBranchInst);
    }

    void LlvmLoweringPass::visitCallInst(std::shared_ptr<CallInst> node) {
        this->requireModule();
        this->requireBuilder();

        std::shared_ptr<Construct> callee = node->callee;
        ConstructKind calleeConstructKind = callee->constructKind;

        if (calleeConstructKind != ConstructKind::Function && calleeConstructKind != ConstructKind::Extern) {
            // TODO: Use DiagnosticBuilder: internal error.
            throw std::runtime_error("Callee is neither a function nor an extern");
        }

        // TODO: ~~~Review with NEW symbol table system~~~ 10/18/2020
        /**
         * If the callee function or extern  has not yet been visited/emitted
         * yet, visit it at this point, as it is required to be present on the
         * emitted entities symbol table in order to emit the call instruction.
         */
        if (!this->valueSymbolTable.contains(callee)) {
            this->saveThenRestoreBuffers([&, this] {
                this->visit(callee);
            });
        }

        std::shared_ptr<Prototype> calleePrototype =
            calleeConstructKind == ConstructKind::Function
                ? callee->dynamicCast<Function>()->prototype
                : callee->dynamicCast<Extern>()->prototype;

        // Attempt to resolve the callee LLVM-equivalent function.
        llvm::Function* llvmCallee =
            (*this->buffers.llvmModule)->getFunction(calleePrototype->name);

        // LLVM-equivalent function could not be found. Report an error.
        if (llvmCallee == nullptr) {
            throw std::runtime_error("Call instruction referenced an undefined function");
        }

        std::vector<std::shared_ptr<Construct>> args = node->args;
        std::vector<llvm::Value*> llvmArgs{};

        for (const auto& arg : args) {
            llvmArgs.push_back(this->valueSafeEarlyVisitOrLookup(arg).get());
        }

        this->valueSymbolTable.set(
            node,

            std::shared_ptr<llvm::Value>(
                this->makeLlvmBuilder()->CreateCall(llvmCallee, llvmArgs)
            )
        );
//        this->addToScope(node, callInst);
    }

    void LlvmLoweringPass::visitStoreInst(std::shared_ptr<StoreInst> node) {
        this->requireFunction();
        this->requireBuilder();

        std::optional<llvm::AllocaInst*> llvmTargetAlloca =
            this->valueSymbolTable.find<llvm::AllocaInst>(node->target);

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

    void LlvmLoweringPass::visitJumpInst(std::shared_ptr<JumpInst> node) {
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

        std::shared_ptr<BasicBlock> basicBlockTarget = node->basicBlockTarget;

        this->saveThenRestoreBuffers([&, this] {
            /**
     * Visit the basic block. A mechanism will prevent it from being
     * emitted twice. Try popping the resulting value because it might
     * be processed if it has been previously emitted.
     */
                this->visitBasicBlock(basicBlockTarget);
                this->valueStack.tryPop();
            }
        );

        std::optional<llvm::BasicBlock*> llvmBasicBlockResult =
            this->valueSymbolTable.find<llvm::BasicBlock>(basicBlockTarget);

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
}
