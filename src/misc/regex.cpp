#include <ionir/misc/regex.h>

namespace ionir {
    std::regex Regex::identifier = std::regex("^([_a-zA-Z]+[_a-zA-Z0-9]*)");

    std::regex Regex::string = std::regex("^\"([^\\\"]*)\"");

    std::regex Regex::decimal = std::regex("^([0-9]+\\.[0-9]+)");

    std::regex Regex::integer = std::regex("^([0-9]+)");

    std::regex Regex::character = std::regex("^'([^'\\n\\\\]{0,1})'");

    std::regex Regex::whitespace = std::regex("^([\\s]+)");
}
