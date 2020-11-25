#pragma once

#include "construct.h"

namespace ionir {
    struct Pass;

    struct AttributeDecl : Construct {
        std::shared_ptr<Prototype> prototype;

        explicit Extern(std::shared_ptr<Prototype> prototype) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
