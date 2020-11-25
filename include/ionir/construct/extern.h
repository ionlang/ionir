#pragma once

#include <ionir/misc/with_parent.h>
#include "construct.h"
#include "prototype.h"

namespace ionir {
    struct Pass;

    struct Extern : Construct, WithParent<Module> {
        std::shared_ptr<Prototype> prototype;

        explicit Extern(std::shared_ptr<Prototype> prototype) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildren() override;
    };
}
