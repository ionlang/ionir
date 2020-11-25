#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    struct TypeVoid : Type {
        TypeVoid() noexcept;

        void accept(Pass& pass) override;
    };
}
