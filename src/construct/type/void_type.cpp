#include <ionir/passes/pass.h>

namespace ionir {
    VoidType::VoidType() noexcept :
        Type(TypeKind::Void, ConstName::typeVoid) {
        //
    }

    void VoidType::accept(Pass& pass) {
        return pass.visitVoidType(this->dynamicCast<VoidType>());
    }
}
