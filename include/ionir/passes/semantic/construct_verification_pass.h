#pragma once

#include <ionir/passes/pass.h>

namespace ionir {
    struct ConstructVerificationPass : Pass {
        IONSHARED_PASS_ID;

        explicit ConstructVerificationPass(
            std::shared_ptr<ionshared::PassContext> context
        ) noexcept;

        void visit(std::shared_ptr<Construct> node) override;
    };
}
