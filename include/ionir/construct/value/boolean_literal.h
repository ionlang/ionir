#pragma once

#include <ionir/construct/value.h>

namespace ionir {
    struct Pass;

    struct BooleanLiteral : Value<> {
        bool value;

        explicit BooleanLiteral(bool value);

        void accept(Pass& visitor) override;
    };
}
