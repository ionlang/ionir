#include <ionir/passes/semantic/construct_verification_pass.h>

namespace ionir {
    ConstructVerificationPass::ConstructVerificationPass(
        ionshared::Ptr<ionshared::PassContext> context
    ) noexcept :
        Pass(std::move(context)) {
        //
    }

    void ConstructVerificationPass::visit(ionshared::Ptr<Construct> node) {
        if (!node->verify()) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::constructFailedVerification)
                ->setSourceLocation(node->sourceLocation)
                ->finish();
        }
    }
}
