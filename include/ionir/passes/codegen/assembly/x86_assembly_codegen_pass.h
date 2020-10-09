#pragma once

#include <ionir/codegen/assembly/x86_assembly_builder.h>
#include <ionir/passes/pass.h>

namespace ionir {
    class x86AssemblyCodegenPass : public Pass {
    private:
        x86AssemblyBuilder builder;

    public:
        IONSHARED_PASS_ID;

        void visitFunction(ionshared::Ptr<Function> node) override;

        void visitJumpInst(ionshared::Ptr<JumpInst> node) override;
    };
}
