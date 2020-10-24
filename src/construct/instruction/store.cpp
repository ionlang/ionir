#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<StoreInst> StoreInst::make(
        const std::shared_ptr<Value<>>& value,
        const std::shared_ptr<AllocaInst>& target
    ) noexcept {
        std::shared_ptr<StoreInst> result =
            std::make_shared<StoreInst>(value, target);

        value->parent = result;

        return result;
    }

    StoreInst::StoreInst(
        std::shared_ptr<Value<>> value,
        std::shared_ptr<AllocaInst> target
    ) noexcept :
        Instruction(InstKind::Store),
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
