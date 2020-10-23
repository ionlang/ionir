#include <utility>
#include <ionir/passes/pass.h>

namespace ionir {
    PointerType::PointerType(std::shared_ptr<Type> type) noexcept :
        Type(TypeKind::Pointer, ConstName::typePointer),
        Wrapper<std::shared_ptr<Type>>(std::move(type)) {
        this->value->parent = this->nativeCast();
    }

    void PointerType::accept(Pass& pass) {
        pass.visitPointerType(this->dynamicCast<PointerType>());
    }
}
