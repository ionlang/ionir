#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    struct BooleanType : Type {
        BooleanType();

        void accept(Pass& pass) override;
    };
}
