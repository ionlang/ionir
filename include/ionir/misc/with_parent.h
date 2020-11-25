#pragma once

#include <ionshared/misc/util.h>

namespace ionir {
    template<typename T>
    class WithParent {
    protected:
        std::optional<std::shared_ptr<T>> parent;

    public:
        WithParent() :
            parent(std::nullopt) {
            //
        }

        std::optional<std::shared_ptr<T>> getParent() const noexcept {
            return this->parent;
        }

        std::shared_ptr<T> forceGetParent() const {
            if (!ionshared::util::hasValue(this->parent)) {
                throw std::runtime_error("Required parent is unset");
            }

            return *this->parent;
        }
    };
}
