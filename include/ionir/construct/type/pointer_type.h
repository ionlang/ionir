#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    struct PointerType : Type, ionshared::Wrapper<std::shared_ptr<Type>> {
        explicit PointerType(std::shared_ptr<Type> type) noexcept;

        void accept(Pass& pass) override;
    };
}
