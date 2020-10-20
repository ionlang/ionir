#include <ionir/passes/pass.h>

namespace ionir {
    Prototype::Prototype(
        std::string name,
        std::shared_ptr<Args> args,
        std::shared_ptr<Type> returnType,
        std::shared_ptr<Module> parent
    ) noexcept :
        ConstructWithParent<Module>(
            std::move(parent),
            ConstructKind::Prototype
        ),

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
}
