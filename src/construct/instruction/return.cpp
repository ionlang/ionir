#include <ionir/passes/pass.h>

namespace ionir {
    InstReturn::InstReturn(
        ionshared::OptPtr<Value<>> value
    ) noexcept :
        Instruction(InstructionKind::Return),
        value(std::move(value)) {
        //
    }

    void InstReturn::accept(Pass& visitor) {
        visitor.visitReturnInst(this->dynamicCast<InstReturn>());
    }

    Ast InstReturn::getChildren() {
        if (!ionshared::util::hasValue(this->value)) {
            return Construct::getChildren();
        }

        return {
            *this->value
        };
    }
}
