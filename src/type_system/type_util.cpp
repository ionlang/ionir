#include <ionir/type_system/type_util.h>
#include <ionir/construct/type/integer_type.h>

namespace ionir::type_util {
    bool isAtomicTypesCompatible(TypeKind typeKindA, TypeKind typeKindB) {
        if (typeKindA == TypeKind::UserDefined || typeKindB == TypeKind::UserDefined) {
            throw std::invalid_argument("Neither argument may be user-defined type kind");
        }
        // Void is incompatible with everything.
        else if (typeKindA == TypeKind::Void || typeKindB == TypeKind::Void) {
            return false;
        }
        // Same, non-void atomic types. They are compatible.
        else if (typeKindA == typeKindB) {
            return true;
        }

        // TODO: Missing additional logic. Refer to (https://github.com/DoctorWkt/acwj/tree/master/12_Types_pt1#building-binary-expressions-comparing-types).

        // Anything remaining is compatible.
        return true;
    }

    bool isSameType(
        const ionshared::Ptr<Type>& typeA,
        const ionshared::Ptr<Type>& typeB
    ) {
        TypeKind typeAKind = typeA->typeKind;
        TypeKind typeBKind = typeB->typeKind;

        if (typeAKind != typeBKind) {
            return false;
        }

        switch (typeAKind) {
            case TypeKind::Integer: {
                ionshared::Ptr<IntegerType> integerTypeA = typeA->dynamicCast<IntegerType>();
                ionshared::Ptr<IntegerType> integerTypeB = typeB->dynamicCast<IntegerType>();

                return integerTypeA->integerKind == integerTypeB->integerKind;
            }

            // TODO: Decimal types as well (copy integer code basically).

            case TypeKind::UserDefined: {
                // TODO

                throw std::runtime_error("Not implemented");

                break;
            }

            default: {
                return true;
            }
        }
    }

    bool isIntegerType(
        ionshared::Ptr<Type> type,
        IntegerKind integerKind
    ) {
        if (type->typeKind != TypeKind::Integer) {
            return false;
        }

        return type->dynamicCast<IntegerType>()->integerKind == integerKind;
    }
}
