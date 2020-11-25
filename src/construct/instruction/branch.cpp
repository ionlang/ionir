#include <ionir/passes/pass.h>

namespace ionir {
    InstBranch::InstBranch(
        std::shared_ptr<Construct> condition,
        std::shared_ptr<BasicBlock> consequentBasicBlock,
        std::shared_ptr<BasicBlock> alternativeBasicBlock
    ) noexcept :
        Instruction(InstructionKind::Branch),
        condition(std::move(condition)),
        consequentBasicBlock(std::move(consequentBasicBlock)),
        alternativeBasicBlock(std::move(alternativeBasicBlock)) {
        //
    }

    void InstBranch::accept(Pass& visitor) {
        visitor.visitBranchInst(this->dynamicCast<InstBranch>());
    }

    Ast InstBranch::getChildren() {
        // TODO: What about condition?

        return {
            this->consequentBasicBlock,
            this->alternativeBasicBlock
        };
    }
}
