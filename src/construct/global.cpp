#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<Global> Global::make(
        const std::shared_ptr<Type>& type,
        const std::string& id,
        ionshared::OptPtr<Value<>> value
    ) noexcept {
        std::shared_ptr<Global> result =
            std::make_shared<Global>(type, id, value);

        type->parent = result;

        if (ionshared::util::hasValue(value)) {
            value->get()->parent = result;
        }

        return result;
    }

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
