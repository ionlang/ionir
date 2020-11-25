#include <ionir/passes/pass.h>

namespace ionir {
    InstCast::InstCast(
        std::shared_ptr<Type> type,
        std::shared_ptr<Value<>> value
    ) noexcept :
        Instruction(InstructionKind::Cast),
        type(std::move(type)),
        value(std::move(value)) {
        //
    }

    void InstCast::accept(Pass& visitor) {
        visitor.visitCastInst(this->dynamicCast<InstCast>());
    }

    Ast InstCast::getChildren() {
        return {
            this->type,
            this->value
        };
    }
}
