#include <ionir/passes/semantic/construct_verification_pass.h>

namespace ionir {
    void ConstructVerificationPass::assertVerification(
        bool verification,
        std::optional<ionshared::SourceLocation> sourceLocation
    ) noexcept {
        if (verification) {
            return;
        }

        this->context->diagnosticBuilder
            ->bootstrap(diagnostic::constructFailedVerification)
            ->setSourceLocation(sourceLocation)
            ->finish();
    }

    ConstructVerificationPass::ConstructVerificationPass(
        std::shared_ptr<ionshared::PassContext> context
    ) noexcept :
        Pass(std::move(context)) {
        //
    }

    void ConstructVerificationPass::visit(std::shared_ptr<Construct> construct) {
        this->assertVerification(Construct::verify(construct), construct->sourceLocation);
    }

    void ConstructVerificationPass::visitBasicBlock(std::shared_ptr<BasicBlock> construct) {
        this->assertVerification(construct->hasTerminalInst(), construct->sourceLocation);
    }

    void ConstructVerificationPass::visitFunction(std::shared_ptr<Function> construct) {
        this->assertVerification(!construct->basicBlocks.empty(), construct->sourceLocation);
    }
}
