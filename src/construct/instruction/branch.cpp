#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<BranchInst> BranchInst::make(
        const std::shared_ptr<Construct>& condition,
        const std::shared_ptr<BasicBlock>& consequentBasicBlock,
        const std::shared_ptr<BasicBlock>& alternativeBasicBlock
    ) noexcept {
        std::shared_ptr<BranchInst> result =
            std::make_shared<BranchInst>(condition, consequentBasicBlock, alternativeBasicBlock);

        condition->parent = result;

        return result;
    }

    BranchInst::BranchInst(
        std::shared_ptr<Construct> condition,
        std::shared_ptr<BasicBlock> consequentBasicBlock,
        std::shared_ptr<BasicBlock> alternativeBasicBlock
    ) noexcept :
        Instruction(InstKind::Branch),
        condition(std::move(condition)),
        consequentBasicBlock(std::move(consequentBasicBlock)),
        alternativeBasicBlock(std::move(alternativeBasicBlock)) {
        //
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
