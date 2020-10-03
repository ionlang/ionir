#include <ionir/passes/codegen/assembly/x86_assembly_codegen_pass.h>

namespace ionir {
    void x86AssemblyCodegenPass::visitFunction(ionshared::Ptr<Function> node) {
        this->builder.makeSection(node->prototype->name);
    }

    void x86AssemblyCodegenPass::visitJumpInst(ionshared::Ptr<JumpInst> node) {
        this->builder.makeJmp(node->basicBlockTarget->name);
    }
}
