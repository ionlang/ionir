#pragma once

#include <string>
#include <ionir/construct/type.h>
#include <ionir/construct/instruction.h>

namespace ionir {
    struct Pass;

    struct ReturnInst : Instruction {
        ionshared::OptPtr<Value<>> value;

        explicit ReturnInst(
            const std::shared_ptr<BasicBlock>& parent,
            ionshared::OptPtr<Value<>> value = std::nullopt
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool hasValue() noexcept;
    };
}
