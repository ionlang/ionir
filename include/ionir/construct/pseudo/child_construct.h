#pragma once

#include <ionir/construct/construct.h>

namespace ionir {
    template<typename T = Construct>
//        requires std::derived_from<T, Construct> // TODO: Cannot work in the current system because ConstructWithParent<T> is used where T is a forward decl.
    struct ConstructWithParent : Construct {
        ConstructWithParent(std::shared_ptr<T> parent, ConstructKind kind) noexcept :
            Construct(kind, std::nullopt, parent) {
            //
        }

        std::shared_ptr<T> getUnboxedParent() {
            if (!ionshared::util::hasValue(this->parent)) {
                // TODO: Should be internal error diagnostic.
                throw std::runtime_error("Parent is nullptr");
            }

            return this->parent->get()->template dynamicCast<T>();
        }
    };
}
