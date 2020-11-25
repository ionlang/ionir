#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    enum struct DecimalKind {
        BitSize8 = 8,

        BitSize16 = 16,

        BitSize32 = 32,

        BitSize64 = 64,

        BitSize128 = 128
    };

    struct TypeDecimal : Type {
        const DecimalKind decimalKind;

        bool isSigned;

        explicit TypeDecimal(DecimalKind kind, bool isSigned = true) noexcept;

        void accept(Pass& pass) override;
    };
}
