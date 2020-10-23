#include <ionir/passes/pass.h>

namespace ionir {
    CharLiteral::CharLiteral(char value) noexcept :
        Value(ValueKind::Character, TypeFactory::typeChar()),
        value(value) {
        this->type->parent = this->nativeCast();
    }

    void CharLiteral::accept(Pass& visitor) {
        visitor.visitCharLiteral(this->dynamicCast<CharLiteral>());
    }
}
