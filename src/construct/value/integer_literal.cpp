#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<IntegerLiteral> IntegerLiteral::make(
        const std::shared_ptr<IntegerType>& type,
        int64_t value
    ) noexcept {
        std::shared_ptr<IntegerLiteral> result =
            std::make_shared<IntegerLiteral>(type, value);

        type->parent = result;

        return result;
    }

    IntegerLiteral::IntegerLiteral(
        std::shared_ptr<IntegerType> type,
        int64_t value
    ) noexcept :
        // TODO: Type has no parent.
        Value(ValueKind::Integer, std::move(type)),
        value(value) {
        //
    }

    void IntegerLiteral::accept(Pass& visitor) {
        visitor.visitIntegerLiteral(this->dynamicCast<IntegerLiteral>());
    }
}
