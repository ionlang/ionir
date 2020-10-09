#pragma once

#include <regex>

namespace ionir {
    struct Regex {
        const static std::regex identifier;

        const static std::regex string;

        const static std::regex decimal;

        const static std::regex integer;

        const static std::regex character;

        const static std::regex whitespace;
    };
}
