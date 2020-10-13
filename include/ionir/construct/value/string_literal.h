#pragma once

#include <string>
#include <ionir/construct/value.h>

namespace ionir {
    struct Pass;

    struct StringLiteral : Value<> {
        std::string value;

        explicit StringLiteral(std::string value) noexcept;

        void accept(Pass& visitor) override;
    };
}
