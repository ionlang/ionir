#include <ionir/passes/pass.h>

namespace ionir {
    LiteralChar::LiteralChar(char value) noexcept :
        // TODO: Type has no parent.
        Value(
            ValueKind::Character,
            std::make_shared<TypeInteger>(IntegerKind::Int8, false)
        ),

        value(value) {
        //
    }

    void LiteralChar::accept(Pass& visitor) {
        visitor.visitCharLiteral(this->dynamicCast<LiteralChar>());
    }
}
