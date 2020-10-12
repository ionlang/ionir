#include <ionir/passes/pass.h>

namespace ionir {
    // TODO: Finish init. implementation.
    OperationInst::OperationInst(const OperationInstOpts& opts) noexcept :
        Inst(opts.parent, InstKind::Alloca),
        operatorKind(opts.operatorKind),
        leftSideValue(opts.leftSide),
        rightSideValue(opts.rightSide) {
        //
    }

    void OperationInst::accept(Pass& visitor) {
        visitor.visitOperationInst(this->dynamicCast<OperationInst>());
    }
}
