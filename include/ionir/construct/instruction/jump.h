#pragma once

#include <optional>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    struct InstJump : Instruction {
        std::shared_ptr<BasicBlock> basicBlockTarget;

        explicit InstJump(
            std::shared_ptr<BasicBlock> basicBlockTarget
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
