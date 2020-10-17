#pragma once

#include <ionir/construct/type.h>
#include <ionir/construct/type/integer_type.h>

namespace ionir::type_util {
    bool isAtomicTypesCompatible(
        TypeKind typeKindA,
        TypeKind typeKindB
    );

    bool isSameType(
        const ionshared::Ptr<Type>& typeA,
        const ionshared::Ptr<Type>& typeB
    );

    bool isIntegerType(
        ionshared::Ptr<Type> type,
        IntegerKind integerKind
    );
}
