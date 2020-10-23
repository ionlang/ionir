#pragma once

#include <ionir/construct/type.h>
#include <ionir/construct/instruction.h>

namespace ionir {
    struct BasicBlock;

    struct AllocaInst : Instruction {
        std::shared_ptr<Type> type;

        // TODO: Missing support for array-type allocas.
        explicit AllocaInst(std::shared_ptr<Type> type) noexcept;

        void accept(Pass& visitor) override;
    };
}
