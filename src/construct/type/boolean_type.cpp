#include <ionir/passes/pass.h>

namespace ionir {
    BooleanType::BooleanType() :
        Type(ConstName::typeBool, TypeKind::Boolean) {
        //
    }

    void BooleanType::accept(Pass& pass) {
        pass.visitBooleanType(this->dynamicCast<BooleanType>());
    }
}
