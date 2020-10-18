#include <ionir/passes/semantic/construct_verification_pass.h>

namespace ionir {
    ConstructVerificationPass::ConstructVerificationPass(
        std::shared_ptr<ionshared::PassContext> context
    ) noexcept :
        Pass(std::move(context)) {
        //
    }

    void ConstructVerificationPass::visit(std::shared_ptr<Construct> node) {
        if (!node->verify()) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::constructFailedVerification)
                ->setSourceLocation(node->sourceLocation)
                ->finish();
        }
    }
}
