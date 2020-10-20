#include <ionir/passes/pass.h>

namespace ionir {
    // TODO: Finish init. implementation.
    AllocaInst::AllocaInst(
        const std::shared_ptr<BasicBlock>& parent,
        std::shared_ptr<Type> type
    ) noexcept :
        Instruction(parent, InstKind::Alloca),
        type(std::move(type)) {
        //
    }

    void AllocaInst::accept(Pass& visitor) {
        visitor.visitAllocaInst(this->dynamicCast<AllocaInst>());
    }
}
