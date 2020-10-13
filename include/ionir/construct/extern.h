#pragma once

#include "construct.h"
#include "prototype.h"

namespace ionir {
    struct Pass;

    struct Extern : ConstructWithParent<Module> {
        ionshared::Ptr<Prototype> prototype;

        Extern(
            ionshared::Ptr<Module> parent,
            ionshared::Ptr<Prototype> prototype
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
