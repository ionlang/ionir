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
        std::shared_ptr<Construct> self = this->nativeCast();

        type->parent = self;

        if (ionshared::util::hasValue(this->value)) {
            this->value->get()->parent = self;
        }
    }

    void Global::accept(Pass& visitor) {
        visitor.visitGlobal(this->dynamicCast<Global>());
    }

    Ast Global::getChildrenNodes() {
        Ast children = {
            this->type->nativeCast()
        };

        auto e = this->type->nativeCast();

        if (this->value.has_value()) {
            children.push_back(this->value->get()->nativeCast());
        }

        return children;
    }
}
