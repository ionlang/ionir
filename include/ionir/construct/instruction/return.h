#pragma once

#include <string>
#include <ionir/construct/type.h>
#include <ionir/construct/instruction.h>

namespace ionir {
    struct Pass;

    struct ReturnInst : Instruction {
        static std::shared_ptr<ReturnInst> make(
            ionshared::OptPtr<Value<>> value = std::nullopt
        ) noexcept;

        ionshared::OptPtr<Value<>> value;

        explicit ReturnInst(
            ionshared::OptPtr<Value<>> value = std::nullopt
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool hasValue() noexcept;
    };
}
