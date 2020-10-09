#include <ionir/passes/semantic/construct_validation_pass.h>

namespace ionir {
    ConstructValidationPass::ConstructValidationPass(
        ionshared::Ptr<ionshared::PassContext> context
    ) noexcept :
        Pass(std::move(context)) {
        //
    }

    void ConstructValidationPass::visit(ionshared::Ptr<Construct> node) {
        if (!node->verify()) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::constructFailedVerification)
                ->setSourceLocation(node->sourceLocation)
                ->finish();
        }
    }
}
