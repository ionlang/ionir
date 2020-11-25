#include <ionir/passes/pass.h>

namespace ionir {
    // TODO: Finish init. implementation.
    InstAlloca::InstAlloca(std::shared_ptr<Type> type) noexcept :
        Instruction(InstructionKind::Alloca),
        type(std::move(type)) {
        //
    }

    void InstAlloca::accept(Pass& visitor) {
        visitor.visitAllocaInst(this->dynamicCast<InstAlloca>());
    }
}
