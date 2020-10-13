#pragma once

#include <optional>
#include <string>
#include <ionir/construct/construct.h>

namespace ionir {
    // TODO: What if 'pass.h' is never included?
    struct Pass;

    struct ErrorMarker : Construct {
        std::string message;

        explicit ErrorMarker(std::string message);

        void accept(Pass& visitor) override;
    };
}
