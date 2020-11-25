#include <ionir/passes/pass.h>

namespace ionir {
    TypeBoolean::TypeBoolean() noexcept :
        Type(TypeKind::Boolean, ConstName::typeBool) {
        //
    }

    void TypeBoolean::accept(Pass& pass) {
        pass.visitBooleanType(this->dynamicCast<TypeBoolean>());
    }
}
