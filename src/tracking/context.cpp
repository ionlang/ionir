#include <ionir/tracking/context.h>

namespace ionir {
    Context::Context(std::vector<Scope> scopes) : scopes(std::move(scopes)) {
        //
    }

    std::vector<Scope> Context::getScopes() const noexcept {
        return this->scopes;
    }

    void Context::setScopes(std::vector<Scope> scopes) noexcept {
        this->scopes = std::move(scopes);
    }

    void Context::appendScope(const Scope &scope) noexcept {
        // Scope is already on the scopes vector.
        if (ionshared::Util::locateInVector(this->scopes, scope) != std::nullopt) {
            return;
        }

        /**
         * Otherwise, push the scope onto the front of the vector.
         * This will allow vector traversal starting from the last
         * added scope, to the first added one.
         */
        this->scopes.insert(this->scopes.begin(), scope);
    }

    void Context::appendScope(const ionshared::Ptr<ScopeAnchor<>> &scopeAnchor) noexcept {
        this->appendScope(scopeAnchor->getSymbolTable());
    }

    void Context::popScope() {
        if (!this->scopes.empty()) {
            this->scopes.erase(this->scopes.begin());
        }
    }

    ionshared::OptPtr<Construct> Context::findNearest(const std::string &id) {
        /**
         * Scopes vector is already in the correct order because
         * whenever a new scope is pushed, it is added at the beginning
         * instead of the end of the scopes vector, essentially reversing
         * the order from last added to first added.
         */
        for (const auto &scope : this->scopes) {
            if (scope->contains(id)) {
                return scope->lookup(id);
            }
        }

        return std::nullopt;
    }
}