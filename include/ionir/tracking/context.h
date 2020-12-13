#pragma once

#include <vector>
#include <ionshared/tracking/symbol_table.h>
#include <ionir/tracking/scope_anchor.h>
#include <ionir/construct/construct.h>

namespace ionir {
    typedef ionshared::PtrSymbolTable<Construct> Scope;

    struct Context {
        std::vector<Scope> scopes;

        Scope globalScope;

        explicit Context(
            Scope globalScope = ionshared::util::makePtrSymbolTable<Construct>(),
            std::vector<Scope> scopes = {}
        );

        /**
         * Removes the lastly added scope from the scopes list.
         * Nothing will happen if the scopes list is empty.
         */
        void popScope();

        /**
         * Attempt to locateInstruction the nearest (starting from most recent
         * scopes) construct by it's id in the scope symbol tables.
         */
        ionshared::OptPtr<Construct> findNearest(const std::string& id);
    };
}
