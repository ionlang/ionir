#pragma once

#include <ionir/passes/pass.h>

namespace ionir {
    struct DeadCodeEliminationPass : Pass {
        IONSHARED_PASS_ID;

        explicit DeadCodeEliminationPass(
            std::shared_ptr<ionshared::PassContext> context
        ) noexcept;

        void visitBasicBlock(std::shared_ptr<BasicBlock> node) override;

        // TODO: Optimize assignment to a dead variable (alloca but never used).
    };
}
