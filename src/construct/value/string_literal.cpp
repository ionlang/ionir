#include <ionir/passes/pass.h>

namespace ionir {
    StringLiteral::StringLiteral(std::string value) noexcept :
        // TODO: Type has no parent. Type is wrong. Awaiting StringType.
        Value(ValueKind::String, std::make_shared<BooleanType>()),
        value(std::move(value)) {
        //
    }

    void StringLiteral::accept(Pass& visitor) {
        visitor.visitStringLiteral(this->dynamicCast<StringLiteral>());
    }
}
