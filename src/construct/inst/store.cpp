#include <ionir/passes/pass.h>

namespace ionir {
    StoreInst::StoreInst(const StoreInstOpts& opts) :
        Inst(opts.parent, InstKind::Store),
        value(opts.value),
        target(opts.target) {
        //
    }

    void StoreInst::accept(Pass& visitor) {
        visitor.visitStoreInst(this->dynamicCast<StoreInst>());
    }

    Ast StoreInst::getChildrenNodes() {
        return {
            this->value,
            this->target
        };
    }
}
