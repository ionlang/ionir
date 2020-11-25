#pragma once

#include <optional>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    struct InstBranch : Instruction {
        std::shared_ptr<Construct> condition;

        std::shared_ptr<BasicBlock> consequentBasicBlock;

        std::shared_ptr<BasicBlock> alternativeBasicBlock;

        InstBranch(
            std::shared_ptr<Construct> condition,
            std::shared_ptr<BasicBlock> consequentBasicBlock,
            std::shared_ptr<BasicBlock> alternativeBasicBlock
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildren() override;
    };
}
