#pragma once

#include <ionir/construct/type/integer_type.h>
#include <ionir/construct/value.h>

namespace ionir {
    struct Pass;

    struct IntegerLiteral : Value<IntegerType> {
        static std::shared_ptr<IntegerLiteral> make(
            const std::shared_ptr<IntegerType>& type,
            int64_t value
        ) noexcept;

        uint64_t value;

        IntegerLiteral(
            std::shared_ptr<IntegerType> type,
            int64_t value
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
