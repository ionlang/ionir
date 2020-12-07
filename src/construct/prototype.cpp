#include <ionir/passes/pass.h>

namespace ionir {
    Prototype::Prototype(
        std::string name,
        std::shared_ptr<Args> args,
        std::shared_ptr<Type> returnType
    ) noexcept :
        Construct(ConstructKind::Prototype),
        Named{std::move(name)},
        args(std::move(args)),
        returnType(std::move(returnType)) {
        //
    }

    void Prototype::accept(Pass& visitor) {
        visitor.visitPrototype(this->dynamicCast<Prototype>());
    }

    Ast Prototype::getChildren() {
        // TODO: What about 'args'?
        return {
            this->returnType
        };
    }

    std::optional<std::string> Prototype::getName() {
        return this->name;
    }

    bool Prototype::isMain() const noexcept {
        return this->name == ConstName::main;
    }
}
