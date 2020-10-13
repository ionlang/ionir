#pragma once

#include <ionir/construct/type/integer_type.h>
#include <ionir/construct/value.h>

namespace ionir {
    struct Pass;

    struct IntegerLiteral : Value<IntegerType> {
        int64_t value;

        IntegerLiteral(ionshared::Ptr<IntegerType> type, int64_t value) noexcept;

        void accept(Pass& visitor) override;
    };
}
