#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    struct PointerType : Type, ionshared::Wrapper<ionshared::Ptr<Type>> {
        explicit PointerType(ionshared::Ptr<Type> type);

        void accept(Pass& pass) override;
    };
}
