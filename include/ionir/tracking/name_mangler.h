#pragma once

#include <string>
#include <ionir/construct/type/struct.h>
#include <ionir/construct/module.h>
#include <ionir/construct/identifier.h>

namespace ionir {
    class NameMangler {
    public:
        static std::string joinStrings(
            const std::string& stringA,
            const std::string& stringB
        ) noexcept;

        static std::string mangle(const std::string& name) noexcept;

        static std::string mangle(
            const std::shared_ptr<Module>& module,
            const std::string& name
        ) noexcept;

        static std::string mangle(
            const std::shared_ptr<StructType>& structType,
            const std::string& name
        ) noexcept;
    };
}
