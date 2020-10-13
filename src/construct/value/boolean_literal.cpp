#include <ionir/passes/pass.h>

namespace ionir {
    BooleanLiteral::BooleanLiteral(bool value) :
        Value(ValueKind::Boolean, TypeFactory::typeBoolean()),
        value(value) {
        //
    }

    void BooleanLiteral::accept(Pass& visitor) {
        visitor.visitBooleanLiteral(this->dynamicCast<BooleanLiteral>());
    }
}
