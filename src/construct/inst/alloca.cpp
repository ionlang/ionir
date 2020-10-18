#include <ionir/passes/pass.h>

namespace ionir {
    // TODO: Finish init. implementation.
    AllocaInst::AllocaInst(const AllocaInstOpts &opts) :
        Instruction(opts.parent, InstKind::Alloca),
        type(opts.type) {
        //
    }

    void AllocaInst::accept(Pass& visitor) {
        visitor.visitAllocaInst(this->dynamicCast<AllocaInst>());
    }
}
