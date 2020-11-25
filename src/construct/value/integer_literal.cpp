#include <ionir/passes/pass.h>

namespace ionir {
    LiteralInteger::LiteralInteger(
        std::shared_ptr<TypeInteger> type,
        int64_t value
    ) noexcept :
        // TODO: Type has no parent.
        Value(ValueKind::Integer, std::move(type)),
        value(value) {
        //
    }

    void LiteralInteger::accept(Pass& visitor) {
        visitor.visitIntegerLiteral(this->dynamicCast<LiteralInteger>());
    }
}
