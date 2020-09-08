#pragma once

#include <ionshared/misc/helpers.h>
#include <ionir/passes/pass.h>

namespace ionir {
    class EntryPointCheckPass : public Pass {
    private:
        ionshared::OptPtr<Function> entryFunction;

    public:
        EntryPointCheckPass();

        void finish() override;

        void visitFunction(ionshared::Ptr<Function> node) override;
    };
}
