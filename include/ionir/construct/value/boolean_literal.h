#pragma once

#include <ionir/construct/value.h>

namespace ionir {
    struct Pass;

    struct LiteralBoolean : Value<> {
        bool value;

        explicit LiteralBoolean(bool value) noexcept;

        void accept(Pass& visitor) override;
    };
}
