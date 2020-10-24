#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<ReturnInst> ReturnInst::make(
        ionshared::OptPtr<Value<>> value
    ) noexcept {
        std::shared_ptr<ReturnInst> result =
            std::make_shared<ReturnInst>(value);

        if (ionshared::util::hasValue(value)) {
            value->get()->parent = result;
        }

        return result;
    }

    ReturnInst::ReturnInst(
        ionshared::OptPtr<Value<>> value
    ) noexcept :
        Instruction(InstKind::Return),
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
