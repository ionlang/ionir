#pragma once

#include <optional>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    struct BranchInst : Instruction {
        static std::shared_ptr<BranchInst> make(
            const std::shared_ptr<Construct>& condition,
            const std::shared_ptr<BasicBlock>& consequentBasicBlock,
            const std::shared_ptr<BasicBlock>& alternativeBasicBlock
        ) noexcept;

        std::shared_ptr<Construct> condition;

        std::shared_ptr<BasicBlock> consequentBasicBlock;

        std::shared_ptr<BasicBlock> alternativeBasicBlock;

        BranchInst(
            std::shared_ptr<Construct> condition,
            std::shared_ptr<BasicBlock> consequentBasicBlock,
            std::shared_ptr<BasicBlock> alternativeBasicBlock
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
