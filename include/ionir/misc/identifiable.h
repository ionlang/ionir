#pragma once

#include <ionshared/misc/helpers.h>

namespace ionir {
    struct Identifier;

    struct Identifiable {
        std::shared_ptr<Identifier> identifier;

        explicit Identifiable(
            std::shared_ptr<Identifier> identifier
        ) noexcept;
    };
}
