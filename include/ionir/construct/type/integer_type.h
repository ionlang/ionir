#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    enum struct IntegerKind {
        Int8 = 8,

        Int16 = 16,

        Int32 = 32,

        Int64 = 64,

        Int128 = 128
    };

    struct TypeInteger : Type {
        const IntegerKind integerKind;

        bool isSigned;

        explicit TypeInteger(IntegerKind kind, bool isSigned = true) noexcept;

        void accept(Pass& pass) override;
    };
}
