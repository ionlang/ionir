#include <ionir/passes/pass.h>

namespace ionir {
    CompareInst::CompareInst() noexcept :
        Instruction(InstKind::Compare) {
        //
    }

    void CompareInst::accept(Pass& visitor) {
        visitor.visitCompareInst(this->dynamicCast<CompareInst>());
    }
}
