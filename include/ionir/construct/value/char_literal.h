#pragma once

#include <ionir/construct/value.h>

namespace ionir {
    struct Pass;

    struct CharLiteral : Value<> {
        char value;

        explicit CharLiteral(char value) noexcept;

        void accept(Pass& visitor) override;
    };
}
