#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<AllocaInst> AllocaInst::make(
        const std::shared_ptr<Type>& type
    ) noexcept {
        std::shared_ptr<AllocaInst> result =
            std::make_shared<AllocaInst>(type);

        type->parent = result;

        return result;
    }

    // TODO: Finish init. implementation.
    AllocaInst::AllocaInst(std::shared_ptr<Type> type) noexcept :
        Instruction(InstructionKind::Alloca),
        type(std::move(type)) {
        //
    }

    void AllocaInst::accept(Pass& visitor) {
        visitor.visitAllocaInst(this->dynamicCast<AllocaInst>());
    }
}
