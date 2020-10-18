#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct ArrayDescriptor {
        std::shared_ptr<Type> type;

        uint32_t size;
    };
}
