#pragma once

#include <ionir/construct/instruction.h>

namespace ionir {
    struct Pass;

    struct Function;

    // TODO: Implement properly.
    struct InstCompare : Instruction {
        explicit InstCompare() noexcept;

        void accept(Pass& visitor) override;
    };
}
