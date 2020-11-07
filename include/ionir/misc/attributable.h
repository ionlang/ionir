#pragma once

#include <string>
#include <ionshared/container/map.h>

namespace ionir {
    struct Attributable {
        ionshared::Map<std::string, Attribute> attributes;
    };
}
