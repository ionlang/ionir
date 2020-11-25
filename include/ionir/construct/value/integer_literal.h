#pragma once

#include <ionir/construct/type/integer_type.h>
#include <ionir/construct/value.h>

namespace ionir {
    struct Pass;

    struct LiteralInteger : Value<TypeInteger> {
        uint64_t value;

        LiteralInteger(
            std::shared_ptr<TypeInteger> type,
            int64_t value
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
