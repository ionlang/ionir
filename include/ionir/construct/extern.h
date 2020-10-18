#pragma once

#include "construct.h"
#include "prototype.h"

namespace ionir {
    struct Pass;

    struct Extern : ConstructWithParent<Module> {
        std::shared_ptr<Prototype> prototype;

        Extern(
            std::shared_ptr<Module> parent,
            std::shared_ptr<Prototype> prototype
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
