#pragma once

#include <ionir/construct/value.h>

namespace ionir {
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

    // TODO: Consider making template<T type>, to constrain left & right side values' types.
    struct OperationValue : Value<> {
        static std::shared_ptr<OperationValue> make(
            OperatorKind operatorKind,
            const std::shared_ptr<Value<>>& leftSideValue,
            ionshared::OptPtr<Value<>> rightSideValue = std::nullopt
        ) noexcept;

        const OperatorKind operatorKind;

        std::shared_ptr<Value<>> leftSideValue;

        ionshared::OptPtr<Value<>> rightSideValue;

        /**
         * The constructor assumes that both left side and right
         * side values' types are the same, since there are no
         * implicit conversions done by the compiler.
         */
        OperationValue(
            OperatorKind operatorKind,
            std::shared_ptr<Value<>> leftSideValue,
            ionshared::OptPtr<Value<>> rightSideValue = std::nullopt
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
