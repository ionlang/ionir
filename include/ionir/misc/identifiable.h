#pragma once

#include <ionshared/misc/helpers.h>

namespace ionir {
    class Identifier;

    struct Identifiable {
        ionshared::Ptr<Identifier> identifier;

        explicit Identifiable(ionshared::Ptr<Identifier> identifier) noexcept;
    };
}
