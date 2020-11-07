#include <ionir/passes/pass.h>

namespace ionir {
    TypeOfIntrinsic::TypeOfIntrinsic(std::shared_ptr<Value<>> value) :
        Construct(ConstructKind::TypeOfIntrinsic),
        value(std::move(value)) {
        //
    }

    void TypeOfIntrinsic::accept(Pass& visitor) {
        visitor.visitTypeOfIntrinsic(this->dynamicCast<TypeOfIntrinsic>());
    }
}
