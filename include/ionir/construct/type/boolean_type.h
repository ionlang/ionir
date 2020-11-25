#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    struct TypeBoolean : Type {
        TypeBoolean() noexcept;

        void accept(Pass& pass) override;
    };
}
