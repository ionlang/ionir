#include <utility>
#include <ionir/passes/pass.h>

namespace ionir {
    PointerType::PointerType(ionshared::Ptr<Type> type) :
        Type(TypeKind::Pointer, ConstName::typePointer),
        Wrapper<ionshared::Ptr<Type>>(std::move(type)) {
        //
    }

    void PointerType::accept(Pass& pass) {
        pass.visitPointerType(this->dynamicCast<PointerType>());
    }
}
