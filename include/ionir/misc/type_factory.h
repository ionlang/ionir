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
        [[nodiscard]] static ionshared::Ptr<IntegerType> typeInteger(
            IntegerKind integerKind,
            bool isSigned = true
        );

        [[nodiscard]] static ionshared::Ptr<BooleanType> typeBoolean();

        [[nodiscard]] static ionshared::Ptr<IntegerType> typeChar();

        [[nodiscard]] static ionshared::Ptr<Type> typeString();

        [[nodiscard]] static ionshared::Ptr<VoidType> typeVoid();
    };
}
