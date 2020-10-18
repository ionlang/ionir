#pragma once

#include <memory>
#include <ionir/construct/type/integer_type.h>
#include <ionir/construct/type/void_type.h>
#include <ionir/construct/type/boolean_type.h>
#include <ionir/construct/construct.h>
#include <ionir/tracking/symbol_table.h>
#include "util.h"

namespace ionir {
    struct TypeFactory {
        [[nodiscard]] static std::shared_ptr<IntegerType> typeInteger(
            IntegerKind integerKind,
            bool isSigned = true
        );

        [[nodiscard]] static std::shared_ptr<BooleanType> typeBoolean();

        [[nodiscard]] static std::shared_ptr<IntegerType> typeChar();

        [[nodiscard]] static std::shared_ptr<Type> typeString();

        [[nodiscard]] static std::shared_ptr<VoidType> typeVoid();
    };
}
