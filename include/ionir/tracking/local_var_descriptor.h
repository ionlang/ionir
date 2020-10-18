#pragma once

#include <string>
#include <ionir/construct/type.h>

namespace ionir {
    struct LocalVariableDescriptor {
        std::string name;

        std::shared_ptr<Type> type;
    };
}
