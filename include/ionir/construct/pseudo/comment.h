#pragma once

#include <optional>
#include <string>

namespace ionir {
    struct Comment {
        std::optional<std::string> text;

        explicit Comment(
            std::optional<std::string> text
        ) noexcept;
    };
}
