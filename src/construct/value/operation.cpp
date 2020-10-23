#include <ionir/passes/pass.h>

namespace ionir {
    // TODO: Finish init. implementation.
    OperationValue::OperationValue(
        OperatorKind operatorKind,
        std::shared_ptr<Value<>> leftSideValue,
        ionshared::OptPtr<Value<>> rightSideValue
    ) noexcept :
        Value<>(ValueKind::Operation, leftSideValue->type),
        operatorKind(operatorKind),
        leftSideValue(std::move(leftSideValue)),
        rightSideValue(std::move(rightSideValue)) {
        std::shared_ptr<Construct> self = this->nativeCast();

        this->leftSideValue->parent = self;

        if (ionshared::util::hasValue(rightSideValue)) {
            this->rightSideValue->get()->parent = self;
        }
    }

    void OperationValue::accept(Pass& visitor) {
        visitor.visitOperationValue(this->dynamicCast<OperationValue>());
    }

    Ast OperationValue::getChildrenNodes() {
        Ast children{
            this->leftSideValue
        };

        if (ionshared::util::hasValue(this->rightSideValue)) {
            children.push_back(*this->rightSideValue);
        }

        return children;
    }
}
