#pragma once

#include <optional>
#include <string>

namespace ionir {
    struct Comment {
        bool isMultiline;

        std::optional<std::string> text;

        Comment(bool isMultiline, std::optional<std::string> text);
    };
}
