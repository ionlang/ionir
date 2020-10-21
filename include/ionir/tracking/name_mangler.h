#pragma once

#include <string>

namespace ionir {
    class NameMangler {
    public:
        static std::string internal(const std::string& id);
    };
}
