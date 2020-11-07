#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<Prototype> Prototype::make(
        const std::string& name,
        const std::shared_ptr<Args>& args,
        const std::shared_ptr<Type>& returnType
    ) noexcept {
        std::shared_ptr<Prototype> result =
            std::make_shared<Prototype>(name, args, returnType);

        returnType->parent = result;

        auto argsNativeMap = args->items->unwrap();

        for (const auto& argument : argsNativeMap) {
            argument.second.first->parent = result;
        }

        return result;
    }

    Prototype::Prototype(
        std::string name,
        std::shared_ptr<Args> args,
        std::shared_ptr<Type> returnType
    ) noexcept :
        ConstructWithParent<Module>(ConstructKind::Prototype),
        Named{std::move(name)},
        args(std::move(args)),
        returnType(std::move(returnType)) {
        //
    }

    void Prototype::accept(Pass& visitor) {
        visitor.visitPrototype(this->dynamicCast<Prototype>());
    }

    Ast Prototype::getChildrenNodes() {
        // TODO: What about 'args'?
        return {
            this->returnType
        };
    }

    bool Prototype::verify() {
        // TODO: Verify args.
        return Construct::verify();
    }

    bool Prototype::isMain() const noexcept {
        return this->name == ConstName::main;
    }
}
