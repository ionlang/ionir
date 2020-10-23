#include <ionir/passes/pass.h>

namespace ionir {
    BooleanLiteral::BooleanLiteral(bool value) noexcept :
        Value(ValueKind::Boolean, TypeFactory::typeBoolean()),
        value(value) {
        this->type->parent = this->nativeCast();
    }

    void BooleanLiteral::accept(Pass& visitor) {
        visitor.visitBooleanLiteral(this->dynamicCast<BooleanLiteral>());
    }
}
