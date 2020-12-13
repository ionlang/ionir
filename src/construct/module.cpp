#include <ionir/passes/pass.h>

namespace ionir {
    bool Module::insertChild(
        const std::string& name,
        const std::shared_ptr<Construct>& childConstruct
    ) noexcept {
        if (this->context->globalScope->contains(name)) {
            return false;
        }

        childConstruct->setModule(this->dynamicCast<Module>());

        this->context->globalScope->set(
            name,
            childConstruct
        );

        return true;
    }

    Module::Module(
        std::shared_ptr<Identifier> identifier,
        std::shared_ptr<Context> context
    ) noexcept :
        Construct(ConstructKind::Module),
        Identifiable(std::move(identifier)),
        context(std::move(context)) {
        //
    }

    void Module::accept(Pass& visitor) {
        visitor.visitModule(this->dynamicCast<Module>());
    }

    Ast Module::getChildren() {
        return Construct::convertChildren(
            // TODO: What about normal scopes? Merge that with global scope. Or actually, module just uses global context, right?
            this->context->globalScope
        );
    }

    bool Module::insert(const std::shared_ptr<Function>& function) {
        return this->insertChild(function->prototype->name, function);
    }

    bool Module::insert(const std::shared_ptr<Extern>& externConstruct) {
        return this->insertChild(externConstruct->prototype->name, externConstruct);
    }

    bool Module::insert(const std::shared_ptr<Global>& global) {
        return this->insertChild(global->name, global);
    }

    bool Module::insert(const std::shared_ptr<TypeStruct>& structType) {
        return this->insertChild(structType->typeName, structType);
    }

    bool Module::mergeInto(const std::shared_ptr<Module>& module) {
        // TODO: What about global scopes? Does this even work properly?

        auto localGlobalScopeMap = this->context->globalScope->unwrap();
        Scope targetGlobalScope = module->context->globalScope;
        Scope newGlobalScope = ionshared::util::makePtrSymbolTable<Construct>();

        // Attempt to merge global scope.
        for (const auto& [key, construct] : localGlobalScopeMap) {
            if (!newGlobalScope->set(key, construct)) {
                return false;
            }
        }

        std::shared_ptr<Context> newContext = std::make_shared<Context>(newGlobalScope);
        std::vector<Scope> targetScopes = module->context->scopes;
        std::vector<Scope> localScopes = this->context->scopes;

        // Attempt to merge scopes.
        for (const auto& scope : localScopes) {
            // TODO: Use ionshared::util::vectorContains<T>(); Does the same thing.
            if (ionshared::util::locateInVector<Scope>(targetScopes, scope) != std::nullopt) {
                return false;
            }

            newContext->scopes.push_back(scope);
        }

        // Update the target module's context.
        module->context = newContext;

        return true;
    }
}
