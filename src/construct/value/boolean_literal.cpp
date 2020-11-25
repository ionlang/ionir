#include <ionir/passes/pass.h>

namespace ionir {
    LiteralBoolean::LiteralBoolean(bool value) noexcept :
        // TODO: Type has no parent.
        Value(ValueKind::Boolean, std::shared_ptr<TypeBoolean>()),
        value(value) {
        //
    }

    void LiteralBoolean::accept(Pass& visitor) {
        visitor.visitBooleanLiteral(this->dynamicCast<LiteralBoolean>());
    }
}
