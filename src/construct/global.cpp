#include <ionir/passes/pass.h>

namespace ionir {
    Global::Global(
        std::shared_ptr<Type> type,
        std::string id,
        ionshared::OptPtr<Value<>> value
    ) noexcept :
        Construct(ConstructKind::Global),
        Named{std::move(id)},
        type(std::move(type)),
        value(std::move(value)) {
        //
    }

    void Global::accept(Pass& visitor) {
        visitor.visitGlobal(this->dynamicCast<Global>());
    }

    Ast Global::getChildren() {
        Ast children = {
            this->type->nativeCast()
        };

        auto e = this->type->nativeCast();

        if (this->value.has_value()) {
            children.push_back(this->value->get()->nativeCast());
        }

        return children;
    }

    std::optional<std::string> Global::getName() {
        return this->name;
    }
}
