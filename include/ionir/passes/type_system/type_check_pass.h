#pragma once

#include <ionir/passes/pass.h>

namespace ionir {
    struct TypeCheckPass : Pass {
        IONSHARED_PASS_ID;

        explicit TypeCheckPass(std::shared_ptr<ionshared::PassContext> context);

        bool initialize(ionshared::PassInfo& info) override;

        void visitFunction(std::shared_ptr<Function> construct) override;

        void visitReturnInst(std::shared_ptr<ReturnInst> construct) override;

        void visitStoreInst(std::shared_ptr<StoreInst> construct) override;

        void visitStructDefinition(std::shared_ptr<StructDefinition> construct) override;

        void visitCallInst(std::shared_ptr<CallInst> construct) override;
    };
}
