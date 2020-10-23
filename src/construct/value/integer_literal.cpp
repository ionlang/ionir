#include <ionir/passes/pass.h>

namespace ionir {
    IntegerLiteral::IntegerLiteral(
        std::shared_ptr<IntegerType> type,
        int64_t value
    ) noexcept :
        Value(ValueKind::Integer, std::move(type)),
        value(value) {
        this->type->parent = this->nativeCast();
    }

    void IntegerLiteral::accept(Pass& visitor) {
        visitor.visitIntegerLiteral(this->dynamicCast<IntegerLiteral>());
    }
}
