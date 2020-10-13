#include <ionir/passes/pass.h>

namespace ionir {
    CallInst::CallInst(const CallInstOpts& opts) :
        Inst(opts.parent, InstKind::Call),
        callee(opts.callee),
        args(opts.args) {
        //
    }

    void CallInst::accept(Pass& visitor) {
        visitor.visitCallInst(this->dynamicCast<CallInst>());
    }
}
