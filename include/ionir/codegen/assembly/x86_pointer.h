#pragma once

#include <string>

namespace ionir {
    enum class x86PointerKind {
        Bit16,

        Bit32
    };

    struct x86Pointer {
        x86PointerKind kind;

        std::string value;
    };
}
