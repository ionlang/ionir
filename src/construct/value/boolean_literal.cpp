#include <ionir/passes/pass.h>

namespace ionir {
    BooleanLiteral::BooleanLiteral(bool value) noexcept :
        // TODO: Type has no parent.
        Value(ValueKind::Boolean, std::shared_ptr<BooleanType>()),
        value(value) {
        //
    }

    void BooleanLiteral::accept(Pass& visitor) {
        visitor.visitBooleanLiteral(this->dynamicCast<BooleanLiteral>());
    }
}
