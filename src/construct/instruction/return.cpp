#include <ionir/passes/pass.h>

namespace ionir {
    ReturnInst::ReturnInst(
        const std::shared_ptr<BasicBlock>& parent,
        ionshared::OptPtr<Value<>> value
    ) noexcept :
        Instruction(parent, InstKind::Return),
        value(std::move(value)) {
        //
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
