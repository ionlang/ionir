#pragma once

#include <ionir/construct/type.h>
#include <ionir/construct/type/integer_type.h>

namespace ionir::type_util {
    bool isAtomicTypesCompatible(
        TypeKind typeKindA,
        TypeKind typeKindB
    );

    bool isIntegerType(
        const std::shared_ptr<Type>& type,
        IntegerKind integerKind
    );
}
