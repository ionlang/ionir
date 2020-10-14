#pragma once

#include <ionir/passes/pass.h>

namespace ionir {
    struct ConstructVerificationPass : Pass {
        IONSHARED_PASS_ID;

        explicit ConstructVerificationPass(
            ionshared::Ptr<ionshared::PassContext> context
        ) noexcept;

        void visit(ionshared::Ptr<Construct> node) override;
    };
}
