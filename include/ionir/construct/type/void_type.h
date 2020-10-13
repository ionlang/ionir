#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    struct VoidType : Type {
        VoidType() noexcept;

        void accept(Pass& pass) override;
    };
}
