#pragma once

#include <string>
#include <ionir/construct/instruction.h>

namespace ionir {
    struct Pass;

    struct Function;

    struct CastInst : Instruction {
        static std::shared_ptr<CastInst> make(
            const std::shared_ptr<Type>& type,
            const std::shared_ptr<Value<>>& value
        ) noexcept;

        std::shared_ptr<Type> type;

        std::shared_ptr<Value<>> value;

        explicit CastInst(
            std::shared_ptr<Type> type,
            std::shared_ptr<Value<>> value
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
