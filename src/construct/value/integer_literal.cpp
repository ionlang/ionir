#include <ionir/passes/pass.h>

namespace ionir {
    IntegerLiteral::IntegerLiteral(ionshared::Ptr<IntegerType> type, int64_t value) noexcept :
        Value(ValueKind::Integer, std::move(type)),
        value(value) {
        //
    }

    void IntegerLiteral::accept(Pass& visitor) {
        visitor.visitIntegerLiteral(this->dynamicCast<IntegerLiteral>());
    }
}
