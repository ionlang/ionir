#include <ionir/passes/pass.h>

namespace ionir {
    // TODO: Finish init. implementation.
    OperationValue::OperationValue(
        OperatorKind operatorKind,
        std::shared_ptr<Value<>> leftSideValue,
        ionshared::OptPtr<Value<>> rightSideValue
    ) noexcept :
        // TODO: Type is borrowed.
        Value<>(ValueKind::Operation, leftSideValue->type),
        operatorKind(operatorKind),
        leftSideValue(std::move(leftSideValue)),
        rightSideValue(std::move(rightSideValue)) {
        //
    }

    void OperationValue::accept(Pass& visitor) {
        visitor.visitOperationValue(this->dynamicCast<OperationValue>());
    }

    Ast OperationValue::getChildrenNodes() {
        Ast children = Value<>::getChildrenNodes();

        if (ionshared::util::hasValue(this->rightSideValue)) {
            children.push_back(*this->rightSideValue);
        }

        return children;
    }
}
