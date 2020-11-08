#include <ionir/passes/pass.h>

namespace ionir {
    CompareInst::CompareInst() noexcept :
        Instruction(InstructionKind::Compare) {
        //
    }

    void CompareInst::accept(Pass& visitor) {
        visitor.visitCompareInst(this->dynamicCast<CompareInst>());
    }
}
