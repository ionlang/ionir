#pragma once

#include <ionshared/misc/helpers.h>

namespace ionir {
    class Identifier;

    struct Identifiable {
        std::shared_ptr<Identifier> identifier;

        explicit Identifiable(std::shared_ptr<Identifier> identifier) noexcept;
    };
}
