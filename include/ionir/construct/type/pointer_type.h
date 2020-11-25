#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    // TODO: Avoid using a Wrapper in this case?
    struct TypePointer : Type, ionshared::Wrapper<std::shared_ptr<Type>> {
        explicit TypePointer(std::shared_ptr<Type> type) noexcept;

        void accept(Pass& pass) override;

        [[nodiscard]] Ast getChildren() override;
    };
}
