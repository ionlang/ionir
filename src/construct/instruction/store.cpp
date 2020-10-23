#include <ionir/passes/pass.h>

namespace ionir {
    StoreInst::StoreInst(
        std::shared_ptr<Value<>> value,
        std::shared_ptr<AllocaInst> target
    ) noexcept :
        Instruction(InstKind::Store),
        value(std::move(value)),
        target(std::move(target)) {
        this->value->parent = this->nativeCast();
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
