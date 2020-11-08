#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<CastInst> CastInst::make(
        const std::shared_ptr<Type>& type,
        const std::shared_ptr<Value<>>& value) noexcept {
        std::shared_ptr<CastInst> result =
            std::make_shared<CastInst>(type, value);

        type->parent = result;
        value->parent = result;

        return result;
    }

    CastInst::CastInst(
        std::shared_ptr<Type> type,
        std::shared_ptr<Value<>> value
    ) noexcept :
        Instruction(InstructionKind::Cast),
        type(std::move(type)),
        value(std::move(value)) {
        //
    }

    void CastInst::accept(Pass& visitor) {
        visitor.visitCastInst(this->dynamicCast<CastInst>());
    }

    Ast CastInst::getChildrenNodes() {
        return {
            this->type,
            this->value
        };
    }
}
