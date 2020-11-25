#include <ionir/passes/pass.h>

namespace ionir {
    TypeInteger::TypeInteger(IntegerKind kind, bool isSigned) noexcept :
        Type(TypeKind::Integer, util::resolveIntegerKindName(kind)),
        integerKind(kind),
        isSigned(isSigned) {
        //
    }

    void TypeInteger::accept(Pass& pass) {
        return pass.visitIntegerType(this->dynamicCast<TypeInteger>());
    }
}
