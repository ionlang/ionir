#include <ionir/passes/pass.h>

namespace ionir {
    DecimalType::DecimalType(DecimalKind kind, bool isSigned) noexcept :
        Type(TypeKind::Decimal, util::resolveDecimalKindName(kind)),
        integerKind(kind),
        isSigned(isSigned) {
        //
    }

    void DecimalType::accept(Pass& pass) {
        return pass.visitDecimalType(this->dynamicCast<DecimalType>());
    }
}
