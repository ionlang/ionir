#pragma once

#include <ionshared/misc/helpers.h>
#include <ionir/passes/pass.h>

namespace ionir {
    class EntryPointCheckPass : public Pass {
    private:
        ionshared::OptPtr<Function> entryFunction;

    public:
        IONSHARED_PASS_ID;

        explicit EntryPointCheckPass(
            std::shared_ptr<ionshared::PassContext> context
        ) noexcept;

        void finish() override;

        void visitFunction(std::shared_ptr<Function> node) override;
    };
}
