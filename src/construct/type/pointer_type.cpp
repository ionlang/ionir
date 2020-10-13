#include <utility>
#include <ionir/passes/pass.h>

namespace ionir {
    PointerType::PointerType(ionshared::Ptr<Type> type) :
        Type(ConstName::typePointer, TypeKind::Pointer),
        Wrapper<ionshared::Ptr<Type>>(std::move(type)) {
        //
    }

    void PointerType::accept(Pass& pass) {
        pass.visitPointerType(this->dynamicCast<PointerType>());
    }
}
