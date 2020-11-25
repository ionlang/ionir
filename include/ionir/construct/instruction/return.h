#pragma once

#include <string>
#include <ionir/construct/type.h>
#include <ionir/construct/instruction.h>

namespace ionir {
    struct Pass;

    struct InstReturn : Instruction {
        ionshared::OptPtr<Value<>> value;

        explicit InstReturn(
            ionshared::OptPtr<Value<>> value = std::nullopt
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildren() override;
    };
}
