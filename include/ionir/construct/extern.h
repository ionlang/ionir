#pragma once

#include "construct.h"
#include "prototype.h"

namespace ionir {
    struct Pass;

    struct Extern : ConstructWithParent<Module> {
        static std::shared_ptr<Extern> make(
            const std::shared_ptr<Prototype>& prototype
        ) noexcept;

        std::shared_ptr<Prototype> prototype;

        explicit Extern(std::shared_ptr<Prototype> prototype) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
