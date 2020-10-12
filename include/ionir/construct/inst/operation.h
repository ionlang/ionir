#pragma once

#include <ionir/construct/inst.h>

namespace ionir {
    class BasicBlock;

    enum struct OperatorKind {
        Equal,

        NotEqual,

        Addition,

        Subtraction,

        Multiplication,

        Division,

        Exponent,

        Modulo,

        LessThan,

        GreaterThan,

        LessThanOrEqualTo,

        GreaterThanOrEqualTo,

        And,

        Or,

        Not
    };

    struct OperationInstOpts : InstOpts {
        OperatorKind operatorKind;

        ionshared::Ptr<Value<>> leftSide;

        ionshared::OptPtr<Value<>> rightSide;
    };

    struct OperationInst : Inst {
        const OperatorKind operatorKind;

        ionshared::Ptr<Value<>> leftSideValue;

        ionshared::OptPtr<Value<>> rightSideValue;

        explicit OperationInst(const OperationInstOpts& opts) noexcept;

        void accept(Pass& visitor) override;
    };
}
