#include <ionir/passes/pass.h>

namespace ionir {
    // TODO: Finish init. implementation.
    AllocaInst::AllocaInst(std::shared_ptr<Type> type) noexcept :
        Instruction(InstKind::Alloca),
        type(std::move(type)) {
        this->type->parent = this->nativeCast();
    }

    void AllocaInst::accept(Pass& visitor) {
        visitor.visitAllocaInst(this->dynamicCast<AllocaInst>());
    }
}
