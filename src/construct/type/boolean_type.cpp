#include <ionir/passes/pass.h>

namespace ionir {
    BooleanType::BooleanType() :
        Type(TypeKind::Boolean, ConstName::typeBool) {
        //
    }

    void BooleanType::accept(Pass& pass) {
        pass.visitBooleanType(this->dynamicCast<BooleanType>());
    }
}
