#include <ionir/passes/pass.h>

namespace ionir {
    Extern::Extern(
        std::shared_ptr<Prototype> prototype
    ) noexcept :
        Construct(ConstructKind::Extern),
        prototype(std::move(prototype)) {
        //
    }

    void Extern::accept(Pass& visitor) {
        visitor.visitExtern(this->dynamicCast<Extern>());
    }

    Ast Extern::getChildren() {
        return {
            this->prototype->nativeCast()
        };
    }

    std::optional<std::string> Extern::getName() {
        return this->prototype->name;
    }
}
