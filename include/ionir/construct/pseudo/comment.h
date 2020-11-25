#pragma once

#include <optional>
#include <string>
#include <ionir/construct/construct.h>

namespace ionir {
    struct Comment : Construct {
        std::optional<std::string> text;

        explicit Comment(
            std::optional<std::string> text
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
