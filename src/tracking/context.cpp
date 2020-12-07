#include <ionir/tracking/context.h>

namespace ionir {
    Context::Context(Scope globalScope, std::vector<Scope> scopes)
        : globalScope(std::move(globalScope)), scopes(std::move(scopes)) {
        //
    }

    void Context::popScope() {
        if (!this->scopes.empty()) {
            this->scopes.erase(this->scopes.begin());
        }
    }

    ionshared::OptPtr<Construct> Context::findNearest(const std::string& id) {
        /**
         * Scopes vector is already in the correct order because
         * whenever a new scope is pushed, it is added at the beginning
         * instead of the end of the scopes vector, essentially reversing
         * the order from last added to first added.
         */
        for (const auto& scope : this->scopes) {
            if (scope->contains(id)) {
                return scope->lookup(id);
            }
        }

        // Lastly, check the global scope.
        return this->globalScope->lookup(id);
    }
}
