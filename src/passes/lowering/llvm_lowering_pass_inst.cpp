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

            std::shared_ptr<llvm::Value>(
                this->llvmBuffers.makeBuilder().CreateAlloca(
                    this->eagerVisitType(construct->type).get(),
                    (llvm::Value*)nullptr
                )
            )
        );
    }

    void LlvmLoweringPass::visitReturnInst(std::shared_ptr<ReturnInst> construct) {
        std::shared_ptr<llvm::ReturnInst> llvmReturnInst{
            this->llvmBuffers.makeBuilder().CreateRetVoid()
        };

        if (ionshared::util::hasValue(construct->value)) {
            /**
             * Create the LLVM equivalent return instruction
             * using the buffered builder.
             */
            llvmReturnInst = std::shared_ptr<llvm::ReturnInst>{
                this->llvmBuffers.makeBuilder()
                    .CreateRet(this->eagerVisitValue(*construct->value).get())
            };
        }

        this->valueSymbolTable.set(construct, llvmReturnInst);
    }

    void LlvmLoweringPass::visitBranchInst(std::shared_ptr<BranchInst> construct) {
        this->valueSymbolTable.set(construct, std::shared_ptr<llvm::Value>(
            this->llvmBuffers.makeBuilder().CreateCondBr(
                this->eagerVisitValue(construct->condition).get(),

                this->eagerVisitValue<llvm::BasicBlock>(
                    construct->consequentBasicBlock
                ).get(),

                this->eagerVisitValue<llvm::BasicBlock>(
                    construct->alternativeBasicBlock
                ).get()
            ))
        );
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

        std::shared_ptr<llvm::Function> llvmCallee =
            this->eagerVisitValue<llvm::Function>(construct->callee);

        // LLVM-equivalent function could not be found. Report an error.
        if (llvmCallee == nullptr) {
            throw std::runtime_error("Call instruction referenced an undefined function");
        }

        std::vector<std::shared_ptr<Construct>> args = construct->args;
        std::vector<llvm::Value*> llvmArgs{};

        llvmArgs.reserve(args.size());

        for (const auto& arg : args) {
            llvmArgs.push_back(this->eagerVisitValue(arg).get());
        }

        this->valueSymbolTable.set(
            construct,

            std::shared_ptr<llvm::Value>(
                this->llvmBuffers.makeBuilder().CreateCall(llvmCallee.get(), llvmArgs)
            )
        );
    }

    void LlvmLoweringPass::visitStoreInst(std::shared_ptr<StoreInst> construct) {
        this->valueSymbolTable.set(construct, std::shared_ptr<llvm::Value>{
            this->llvmBuffers.makeBuilder().CreateStore(
                this->eagerVisitValue(construct->value).get(),
                this->eagerVisitValue(construct->target).get()
            )
        });
    }

    void LlvmLoweringPass::visitJumpInst(std::shared_ptr<JumpInst> node) {
        this->valueSymbolTable.set(
            node,

            std::shared_ptr<llvm::Value>{this->llvmBuffers.makeBuilder().CreateBr(
                this->eagerVisitValue<llvm::BasicBlock>(node->basicBlockTarget).get()
            )}
        );
    }
}
