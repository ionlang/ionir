#pragma once

#include <ionir/passes/pass.h>

namespace ionir {
    struct TypeCheckPass : Pass {
        IONSHARED_PASS_ID;

        explicit TypeCheckPass(ionshared::Ptr<ionshared::PassContext> context);

        bool initialize(ionshared::PassInfo &info) override;

        void visitFunction(ionshared::Ptr<Function> node) override;

        void visitReturnInst(ionshared::Ptr<ReturnInst> node) override;

        void visitStoreInst(ionshared::Ptr<StoreInst> node) override;
    };
}
