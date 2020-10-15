#include <ionir/passes/pass.h>

namespace ionir {
    IntegerType::IntegerType(IntegerKind kind, bool isSigned) :
        Type(TypeKind::Integer, util::resolveIntegerKindName(kind)),
        integerKind(kind),
        isSigned(isSigned) {
        //
    }

    void IntegerType::accept(Pass& pass) {
        return pass.visitIntegerType(this->dynamicCast<IntegerType>());
    }
}
