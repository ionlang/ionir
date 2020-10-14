#include <ionir/passes/pass.h>

namespace ionir {
    Function::Function(ionshared::Ptr<Prototype> prototype, ionshared::Ptr<FunctionBody> body) :
        Construct(ConstructKind::Function),
        prototype(std::move(prototype)),
        body(std::move(body)) {
        //
    }

    void Function::accept(Pass &visitor) {
        visitor.visitFunction(this->dynamicCast<Function>());
    }

    Ast Function::getChildrenNodes() {
        return {
            this->prototype,
            this->body
        };
    }
}
