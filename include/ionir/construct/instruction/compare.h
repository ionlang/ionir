#pragma once

#include <ionir/construct/instruction.h>

namespace ionir {
    struct Pass;

    struct Function;

    // TODO: Implement properly.
    struct CompareInst : Instruction {
        explicit CompareInst(
            const std::shared_ptr<BasicBlock>& parent
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
