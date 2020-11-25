#pragma once

#include <string>
#include <ionir/construct/instruction.h>

namespace ionir {
    struct Pass;

    struct Function;

    struct InstCast : Instruction {
        std::shared_ptr<Type> type;

        std::shared_ptr<Value<>> value;

        explicit InstCast(
            std::shared_ptr<Type> type,
            std::shared_ptr<Value<>> value
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
