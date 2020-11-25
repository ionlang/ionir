#include <ionir/passes/pass.h>

namespace ionir {
    InstStore::InstStore(
        std::shared_ptr<Value<>> value,
        std::shared_ptr<InstAlloca> target
    ) noexcept :
        Instruction(InstructionKind::Store),
        value(std::move(value)),
        target(std::move(target)) {
        //
    }

    void InstStore::accept(Pass& visitor) {
        visitor.visitStoreInst(this->dynamicCast<InstStore>());
    }

    Ast InstStore::getChildrenNodes() {
        return {
            this->value,
            this->target
        };
    }
}
