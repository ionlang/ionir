#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    // TODO: Avoid using a Wrapper in this case?
    struct PointerType : Type, ionshared::Wrapper<std::shared_ptr<Type>> {
        static std::shared_ptr<PointerType> make(
            const std::shared_ptr<Type>& type
        ) noexcept;

        explicit PointerType(std::shared_ptr<Type> type) noexcept;

        void accept(Pass& pass) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
