#include <ionir/passes/pass.h>

namespace ionir {
    Struct::Struct(std::string name, Fields fields) noexcept :
        Type(TypeKind::Struct, std::move(name)),
        fields(std::move(fields)) {
        std::shared_ptr<Construct> self = this->nativeCast();
        auto fieldsNativeMap = fields->unwrap();

        for (const auto& [name, type] : fieldsNativeMap) {
            type->parent = self;
        }
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
