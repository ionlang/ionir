#pragma once

#include <ionir/construct/value.h>

namespace ionir {
    struct Pass;

    struct LiteralChar : Value<> {
        char value;

        explicit LiteralChar(char value) noexcept;

        void accept(Pass& visitor) override;
    };
}
