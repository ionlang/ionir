#include <ionir/passes/pass.h>

namespace ionir {
    Struct::Struct(std::string name, Fields fields) noexcept :
        Construct(ConstructKind::Struct),
        ionshared::Named{std::move(name)},
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

    bool Struct::containsField(std::string name) const {
        return this->fields->contains(std::move(name));
    }
}
