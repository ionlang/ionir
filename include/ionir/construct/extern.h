#pragma once

#include <ionir/misc/with_parent.h>
#include "construct.h"
#include "prototype.h"

namespace ionir {
    struct Pass;

    /**
     * A definition to an external function. The prototype name
     * of externs are not mangled in any way during lowering step.
     */
    struct Extern : Construct, WithParent<Module> {
        std::shared_ptr<Prototype> prototype;

        explicit Extern(std::shared_ptr<Prototype> prototype) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildren() override;

        [[nodiscard]] std::optional<std::string> getName() override;
    };
}
