#include <ionir/passes/pass.h>

namespace ionir {
    ReturnInst::ReturnInst(
        ionshared::OptPtr<Value<>> value
    ) noexcept :
        Instruction(InstKind::Return),
        value(std::move(value)) {
        if (ionshared::util::hasValue(this->value)) {
            this->value->get()->parent = this->nativeCast();
        }
    }

    void ReturnInst::accept(Pass& visitor) {
        visitor.visitReturnInst(this->dynamicCast<ReturnInst>());
    }

    Ast ReturnInst::getChildrenNodes() {
        if (!this->hasValue()) {
            return Construct::getChildrenNodes();
        }

        return {
            *this->value
        };
    }

    bool ReturnInst::hasValue() noexcept {
        return ionshared::util::hasValue(this->value);
    }
}
