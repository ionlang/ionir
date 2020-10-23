#include <ionir/passes/pass.h>

namespace ionir {
    BranchInst::BranchInst(
        std::shared_ptr<Construct> condition,
        std::shared_ptr<BasicBlock> consequentBasicBlock,
        std::shared_ptr<BasicBlock> alternativeBasicBlock
    ) noexcept :
        Instruction(InstKind::Branch),
        condition(std::move(condition)),
        consequentBasicBlock(std::move(consequentBasicBlock)),
        alternativeBasicBlock(std::move(alternativeBasicBlock)) {
        this->condition->parent = this->nativeCast();
    }

    void BranchInst::accept(Pass& visitor) {
        visitor.visitBranchInst(this->dynamicCast<BranchInst>());
    }

    Ast BranchInst::getChildrenNodes() {
        // TODO: What about condition?

        return {
            this->consequentBasicBlock,
            this->alternativeBasicBlock
        };
    }
}
