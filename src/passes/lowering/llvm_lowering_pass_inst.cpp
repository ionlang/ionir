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
        llvm::ReturnInst* llvmReturnInst =
            this->llvmBuffers.makeBuilder().CreateRetVoid();

        if (ionshared::util::hasValue(construct->value)) {
            /**
             * Create the LLVM equivalent return instruction
             * using the buffered builder.
             */
            llvmReturnInst = this->llvmBuffers.makeBuilder()
                .CreateRet(this->eagerVisitValue(*construct->value));
        }

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
}
