#pragma once

#include <ionir/construct/type/integer_type.h>
#include <ionir/construct/value.h>

namespace ionir {
    struct Pass;

    struct IntegerLiteral : Value<IntegerType> {
        uint64_t value;

        IntegerLiteral(std::shared_ptr<IntegerType> type, int64_t value) noexcept;

        void accept(Pass& visitor) override;
    };
}
