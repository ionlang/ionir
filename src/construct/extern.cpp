#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<Extern> Extern::make(
        const std::shared_ptr<Prototype>& prototype
    ) noexcept {
        std::shared_ptr<Extern> result =
            std::make_shared<Extern>(prototype);

        prototype->parent = result;

        return result;
    }

    Extern::Extern(
        std::shared_ptr<Prototype> prototype
    ) noexcept :
        ConstructWithParent<Module>(ConstructKind::Extern),
        prototype(std::move(prototype)) {
        //
    }

    void Extern::accept(Pass& visitor) {
        visitor.visitExtern(this->dynamicCast<Extern>());
    }

    Ast Extern::getChildrenNodes() {
        return {
            this->prototype->nativeCast()
        };
    }
}
