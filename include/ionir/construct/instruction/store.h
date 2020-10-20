#pragma once

#include <ionir/construct/instruction.h>
#include <ionir/construct/value.h>
#include <ionir/construct/pseudo/child_construct.h>

namespace ionir {
    struct Pass;

    struct StoreInst : Instruction {
        std::shared_ptr<Value<>> value;

        std::shared_ptr<AllocaInst> target;

        explicit StoreInst(
            const std::shared_ptr<BasicBlock>& parent,
            std::shared_ptr<Value<>> value,
            std::shared_ptr<AllocaInst> target
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
