#pragma once

#include <ionir/tracking/symbol_table.h>
#include "construct.h"
#include "prototype.h"
#include "function_body.h"

namespace ionir {
    struct Pass;

    struct Function : Construct {
        std::shared_ptr<Prototype> prototype;

        std::shared_ptr<FunctionBody> body;

        Function(
            std::shared_ptr<Prototype> prototype,
            std::shared_ptr<FunctionBody> body
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
