#include <ionir/passes/pass.h>

namespace ionir {
    Function::Function(
        std::shared_ptr<Prototype> prototype,
        std::shared_ptr<FunctionBody> body
    ) noexcept :
        Construct(ConstructKind::Function),
        prototype(std::move(prototype)),
        body(std::move(body)) {
        std::shared_ptr<Construct> self = this->nativeCast();

        prototype->parent = self;
        body->parent = self;
    }

    void Function::accept(Pass& visitor) {
        visitor.visitFunction(this->dynamicCast<Function>());
    }

    Ast Function::getChildrenNodes() {
        return {
            this->prototype,
            this->body
        };
    }
}
