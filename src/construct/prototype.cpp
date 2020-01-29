#include <ionir/passes/pass.h>

namespace ionir {
    Prototype::Prototype(std::string id, Ptr<Args> args, Ptr<Type> returnType)
        : Construct(ConstructKind::Prototype), Named(id), args(args), returnType(returnType) {
        //
    }

    void Prototype::accept(Pass &visitor) {
        visitor.visitPrototype(this->dynamicCast<Prototype>());
    }

    Ast Prototype::getChildNodes() const {
        // TODO: What about 'args'?
        return {
            this->returnType->nativeCast()
        };
    }

    Ptr<Args> Prototype::getArgs() const {
        return this->args;
    }

    void Prototype::setArgs(Ptr<Args> args) {
        this->args = args;
    }

    Ptr<Type> Prototype::getReturnType() const {
        return this->returnType;
    }

    void Prototype::setReturnType(Ptr<Type> returnType) {
        this->returnType = returnType;
    }

    bool Prototype::verify() {
        // TODO: Verify args.
        return this->returnType->verify();
    }
}
