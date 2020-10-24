#pragma once

#include <ionir/construct/instruction.h>
#include <ionir/construct/value.h>
#include <ionir/construct/pseudo/child_construct.h>

namespace ionir {
    struct Pass;

    struct StoreInst : Instruction {
        static std::shared_ptr<StoreInst> make(
            const std::shared_ptr<Value<>>& value,
            const std::shared_ptr<AllocaInst>& target
        ) noexcept;

        std::shared_ptr<Value<>> value;

        std::shared_ptr<AllocaInst> target;

        explicit StoreInst(
            std::shared_ptr<Value<>> value,
            std::shared_ptr<AllocaInst> target
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
