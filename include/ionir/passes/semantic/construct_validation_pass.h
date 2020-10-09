#pragma once

#include <ionir/passes/pass.h>

namespace ionir {
    struct ConstructValidationPass : Pass {
        IONSHARED_PASS_ID;

        explicit ConstructValidationPass(
            ionshared::Ptr<ionshared::PassContext> context
        ) noexcept;

        void visit(ionshared::Ptr<Construct> node) override;
    };
}
