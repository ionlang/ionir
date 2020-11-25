#include <ionir/passes/pass.h>

namespace ionir {
    TypeVoid::TypeVoid() noexcept :
        Type(TypeKind::Void, ConstName::typeVoid) {
        //
    }

    void TypeVoid::accept(Pass& pass) {
        return pass.visitVoidType(this->dynamicCast<TypeVoid>());
    }
}
