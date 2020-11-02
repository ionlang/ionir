#include <ionir/passes/pass.h>

namespace ionir {
    CharLiteral::CharLiteral(char value) noexcept :
        // TODO: Type has no parent.
        Value(
            ValueKind::Character,
            std::make_shared<IntegerType>(IntegerKind::Int8, false)
        ),

        value(value) {
        //
    }

    void CharLiteral::accept(Pass& visitor) {
        visitor.visitCharLiteral(this->dynamicCast<CharLiteral>());
    }
}
