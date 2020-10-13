#pragma once

#include <vector>
#include <utility>
#include <string>
#include <ionir/construct/type.h>
#include <ionir/tracking/symbol_table.h>

namespace ionir {
    typedef std::pair<ionshared::Ptr<Type>, std::string> Arg;

    struct Args {
        ionshared::Ptr<ionshared::SymbolTable<Arg>> items;

        bool isVariable;

        explicit Args(
            ionshared::Ptr<ionshared::SymbolTable<Arg>> items =
                std::make_shared<ionshared::SymbolTable<Arg>>(),

            bool isVariable = false
        );
    };
}
