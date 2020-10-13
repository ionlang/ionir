#include <ionir/construct/inst/compare.h>
#include <ionir/passes/pass.h>

namespace ionir {
    CompareInst::CompareInst(const CompareInstOpts &opts) :
        Inst(opts.parent, InstKind::Compare) {
        //
    }

    void CompareInst::accept(Pass &visitor) {
        // TODO: Implement.
//        visitor.visitCallInst(this->dynamicCast<CallInst>());
    }
}
