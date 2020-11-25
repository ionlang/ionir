#include <ionir/passes/pass.h>

namespace ionir {
    TypePointer::TypePointer(std::shared_ptr<Type> type) noexcept :
        Type(TypeKind::Pointer, ConstName::typePointer),
        Wrapper<std::shared_ptr<Type>>(std::move(type)) {
        //
    }

    void TypePointer::accept(Pass& pass) {
        pass.visitPointerType(this->dynamicCast<TypePointer>());
    }

    Ast TypePointer::getChildrenNodes() {
        return {this->value};
    }
}
