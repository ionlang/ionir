#include <ionir/passes/pass.h>

namespace ionir {
    Struct::Struct(std::string name, Fields fields) noexcept :
        Type(TypeKind::Struct, std::move(name)),
        fields(std::move(fields)) {
        //
    }

    void Struct::accept(Pass& visitor) {
        visitor.visitStruct(this->dynamicCast<Struct>());
    }

    Ast Struct::getChildrenNodes() {
        Ast children = {};
        auto fieldsMap = this->fields->unwrap();

        // TODO: What about the field name?
        for (const auto& [name, type] : fieldsMap) {
            children.push_back(type);
        }

        return children;
    }
}
