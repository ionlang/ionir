#include <ionir/passes/pass.h>

namespace ionir {
    TypeDecimal::TypeDecimal(DecimalKind kind, bool isSigned) noexcept :
        Type(TypeKind::Decimal, util::resolveDecimalKindName(kind)),
        integerKind(kind),
        isSigned(isSigned) {
        //
    }

    void TypeDecimal::accept(Pass& pass) {
        return pass.visitDecimalType(this->dynamicCast<TypeDecimal>());
    }
}
