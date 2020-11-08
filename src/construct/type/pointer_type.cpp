#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<PointerType> PointerType::make(
        const std::shared_ptr<Type>& type
    ) noexcept {
        std::shared_ptr<PointerType> result =
            std::make_shared<PointerType>(type);

        type->parent = result;

        return result;
    }

    PointerType::PointerType(std::shared_ptr<Type> type) noexcept :
        Type(TypeKind::Pointer, ConstName::typePointer),
        Wrapper<std::shared_ptr<Type>>(std::move(type)) {
        //
    }

    void PointerType::accept(Pass& pass) {
        pass.visitPointerType(this->dynamicCast<PointerType>());
    }

    Ast PointerType::getChildrenNodes() {
        return {this->value};
    }
}
