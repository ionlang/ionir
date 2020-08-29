#include <ionir/passes/pass.h>

namespace ionir {
    Module::Module(std::string id, ionshared::Ptr<Context> context) :
        Construct(ConstructKind::Module),
        Named(std::move(id)),
        context(std::move(context)) {
        //
    }

    void Module::accept(Pass &visitor) {
        visitor.visitModule(this->dynamicCast<Module>());
    }

    Ast Module::getChildNodes() {
        return Construct::convertChildren(
            // TODO: What about normal scopes? Merge that with global scope. Or actually, module just uses global context, right?
            this->context->getGlobalScope()
        );
    }

    ionshared::Ptr<Context> Module::getContext() const noexcept {
        return this->context;
    }

    void Module::setContext(ionshared::Ptr<Context> context) noexcept {
        this->context = std::move(context);
    }

    void Module::insertFunction(const ionshared::Ptr<Function> &function) {
        // TODO: Check if function exists first?
        this->context->getGlobalScope()->insert(
            function->getPrototype()->getId(),
            function
        );
    }

    ionshared::OptPtr<Function> Module::lookupFunction(std::string id) {
        ionshared::OptPtr<Construct> functionConstruct =
            this->context->getGlobalScope()->lookup(std::move(id));

        if (ionshared::util::hasValue(functionConstruct) && functionConstruct->get()->getConstructKind() == ConstructKind::Function) {
            return functionConstruct->get()->dynamicCast<Function>();
        }

        return std::nullopt;
    }

    bool Module::mergeInto(const ionshared::Ptr<Module> &module) {
        auto localGlobalScopeMap = this->context->getGlobalScope()->unwrap();
        std::vector<Scope> localScopes = this->context->getScopes();
        std::vector<Scope> targetScopes = module->getContext()->getScopes();
        Scope targetGlobalScope = module->getContext()->getGlobalScope();
        Scope newGlobalScope = ionshared::util::makePtrSymbolTable<Construct>();
        ionshared::Ptr<Context> newContext = std::make_shared<Context>(newGlobalScope);

        // Attempt to merge global scope.
        for (const auto &[key, construct] : localGlobalScopeMap) {
            if (!newGlobalScope->insert(key, construct)) {
                return false;
            }
        }

        // Attempt to merge scopes.
        for (const auto &scope : localScopes) {
            // TODO: Use ionshared::util::vectorContains<T>(); Does the same thing.
            if (ionshared::util::locateInVector<Scope>(targetScopes, scope) != std::nullopt) {
                return false;
            }

            newContext->appendScope(scope);
        }

        // Update the target module's context.
        module->setContext(newContext);

        return true;
    }
}
