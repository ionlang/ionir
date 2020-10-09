#include <string>
#include <ionir/type_system/type_util.h>
#include <ionir/passes/semantic/entry_point_check_pass.h>

namespace ionir {
    EntryPointCheckPass::EntryPointCheckPass(
        ionshared::Ptr<ionshared::PassContext> context
    ) noexcept :
        Pass(std::move(context)) {
        //
    }

    void EntryPointCheckPass::finish() {
        /**
         * An entry function could not be found.
         */
        if (!ionshared::util::hasValue(this->entryFunction)) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::entryPointMissing)
                ->finish();

            return;
        }

        /**
         * At this point, the entry point exists. Clear up the entry
         * function as it is no longer needed after the pass has ran.
         */
        this->entryFunction.reset();
    }

    void EntryPointCheckPass::visitFunction(ionshared::Ptr<Function> node) {
        Pass::visitFunction(node);

        if (node->prototype->name == ConstName::main) {
            /**
             * Make sure there was not an entry point function
             * previously encountered.
             */
            if (ionshared::util::hasValue(this->entryFunction)) {
                this->context->diagnosticBuilder
                    ->bootstrap(diagnostic::entryPointRedefinition)
                    ->setSourceLocation(node->sourceLocation)
                    ->finish();

                return;
            }

            this->entryFunction = node;

            bool correctReturnType = type_util::isIntegerType(
                node->prototype->returnType,
                IntegerKind::Int32
            );

            if (!correctReturnType) {
                this->context->diagnosticBuilder
                    ->bootstrap(diagnostic::entryPointInvalidSignature)
                    ->setSourceLocation(node->sourceLocation)
                    ->finish();
            }
        }
    }
}
