#include <ionir/passes/pass.h>

namespace ionir {
    TypeStruct::TypeStruct(std::string name, FieldSymbolTable fields) noexcept :
        Type(TypeKind::Struct, std::move(name)),
        fields(std::move(fields)) {
        //
    }

    void TypeStruct::accept(Pass& visitor) {
        visitor.visitStructType(this->dynamicCast<TypeStruct>());
    }

    Ast TypeStruct::getChildrenNodes() {
        Ast children = {};
        auto fieldsMap = this->fields->unwrap();

        // TODO: What about the field name?
        for (const auto& [name, type] : fieldsMap) {
            children.push_back(type);
        }

        return children;
    }
}
