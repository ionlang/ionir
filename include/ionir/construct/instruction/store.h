#pragma once

#include <ionir/construct/instruction.h>
#include <ionir/construct/value.h>

namespace ionir {
    struct Pass;

    struct InstStore : Instruction {
        std::shared_ptr<Value<>> value;

        std::shared_ptr<InstAlloca> target;

        explicit InstStore(
            std::shared_ptr<Value<>> value,
            std::shared_ptr<InstAlloca> target
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
