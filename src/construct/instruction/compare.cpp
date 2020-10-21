#include <ionir/passes/pass.h>

namespace ionir {
    CompareInst::CompareInst(
        const std::shared_ptr<BasicBlock>& parent
    ) noexcept :
        Instruction(parent, InstKind::Compare) {
        //
    }

    void CompareInst::accept(Pass& visitor) {
        visitor.visitCompareInst(this->dynamicCast<CompareInst>());
    }
}
