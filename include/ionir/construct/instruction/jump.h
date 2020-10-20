#pragma once

#include <optional>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    struct JumpInst : Instruction {
        std::shared_ptr<BasicBlock> basicBlockTarget;

        JumpInst(
            const std::shared_ptr<BasicBlock>& parent,
            std::shared_ptr<BasicBlock> basicBlockTarget
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
