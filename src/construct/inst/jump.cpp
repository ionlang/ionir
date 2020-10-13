#include <ionir/passes/pass.h>

namespace ionir {
    JumpInst::JumpInst(const JumpInstOpts& opts) :
        Inst(opts.parent, InstKind::Jump),
        basicBlockTarget(opts.basicBlockTarget) {
        //
    }

    void JumpInst::accept(Pass& visitor) {
        visitor.visitJumpInst(this->dynamicCast<JumpInst>());
    }

    Ast JumpInst::getChildrenNodes() {
        // TODO: Should have no children, function bodies should be LINEAR! BRANCH defines inline block! WRONG!
        return {
            this->basicBlockTarget
        };
    }
}
