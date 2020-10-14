#pragma once

#include <stdexcept>
#include "wrapper.h"

namespace ionir {
    template<typename T>
    struct SafeWrapper : Wrapper<T> {
        explicit SafeWrapper(T value) noexcept :
            Wrapper<T>(value) {
            if (value == nullptr) {
                throw std::runtime_error("Cannot construct with nullptr value");
            }
        }
    };
}
