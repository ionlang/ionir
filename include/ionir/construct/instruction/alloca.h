#pragma once

#include <ionir/construct/type.h>
#include <ionir/construct/instruction.h>

namespace ionir {
    struct BasicBlock;

    struct AllocaInst : Instruction {
        std::shared_ptr<Type> type;

        // TODO: Missing support for array-type allocas.
        AllocaInst(
            const std::shared_ptr<BasicBlock>& parent,
            std::shared_ptr<Type> type
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
