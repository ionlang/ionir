#pragma once

#include <optional>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    struct JumpInst : Instruction {
        std::shared_ptr<BasicBlock> basicBlockTarget;

        explicit JumpInst(
            std::shared_ptr<BasicBlock> basicBlockTarget
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
