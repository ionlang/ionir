#pragma once

#include <optional>
#include <string>
#include "inst.h"

namespace ionir {
    struct InstYieldOpts : InstOpts {
        std::string yieldName;
    };

    struct InstYield {
        std::string yieldName;

        explicit InstYield(std::string yieldName) noexcept;
    };
}
