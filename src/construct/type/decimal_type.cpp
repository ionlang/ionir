#include <ionir/passes/pass.h>

namespace ionir {
    TypeDecimal::TypeDecimal(DecimalKind kind, bool isSigned) noexcept :
        Type(TypeKind::Decimal, util::resolveDecimalKindName(kind)),
        decimalKind(kind),
        isSigned(isSigned) {
        //
    }

    void TypeDecimal::accept(Pass& pass) {
        return pass.visitDecimalType(this->dynamicCast<TypeDecimal>());
    }
}
