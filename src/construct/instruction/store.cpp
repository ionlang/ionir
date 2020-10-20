#include <ionir/passes/pass.h>

namespace ionir {
    StoreInst::StoreInst(
        const std::shared_ptr<BasicBlock>& parent,
        std::shared_ptr<Value<>> value,
        std::shared_ptr<AllocaInst> target
    ) noexcept :
        Instruction(parent, InstKind::Store),
        value(std::move(value)),
        target(std::move(target)) {
        //
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
