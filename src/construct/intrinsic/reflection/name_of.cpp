#include <ionir/passes/pass.h>

namespace ionir {
    NameOfIntrinsic::NameOfIntrinsic(std::shared_ptr<Type> type) :
        Construct(ConstructKind::IntrinsicNameOf),
        type(std::move(type)) {
        //
    }

    void NameOfIntrinsic::accept(Pass& visitor) {
        visitor.visitNameOfIntrinsic(this->dynamicCast<NameOfIntrinsic>());
    }
}
