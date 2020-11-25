#pragma once

#include <ionir/passes/pass.h>

namespace ionir {
    class ConstructVerificationPass : public Pass {
    private:
        void assertVerification(
            bool verification,
            std::optional<ionshared::SourceLocation> sourceLocation
        ) noexcept;

    public:
        IONSHARED_PASS_ID;

        explicit ConstructVerificationPass(
            std::shared_ptr<ionshared::PassContext> context
        ) noexcept;

        /**
         * Verify the members and properties of the node, and it's children to
         * ensure that this construct is well-formed. Without an implementation
         * by the derived class (or without being called by it), this will return
         * true if all the child nodes are successfully verified. If there are no
         * child nodes, the result will be true by default. Verification should
         * not be delegated to the parent (if any), to avoid circular, endless
         * calls.
         */
        void visit(std::shared_ptr<Construct> node) override;

        void visitBasicBlock(std::shared_ptr<BasicBlock> construct) override;

        void visitFunction(std::shared_ptr<Function> construct) override;
    };
}
