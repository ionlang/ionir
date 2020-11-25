#include <ionir/passes/pass.h>

namespace ionir {
    InstCompare::InstCompare() noexcept :
        Instruction(InstructionKind::Compare) {
        //
    }

    void InstCompare::accept(Pass& visitor) {
        visitor.visitCompareInst(this->dynamicCast<InstCompare>());
    }
}
