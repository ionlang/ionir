#include <ionir/passes/pass.h>

namespace ionir {
    StructDefinition::StructDefinition(
        const std::shared_ptr<TypeStruct>& type,
        std::vector<std::shared_ptr<Value<>>> values
    ) :
        Value<TypeStruct>(
            ValueKind::StructDefinition,
            type
        ),

        values(std::move(values)) {
        //
    }

    void StructDefinition::accept(Pass& visitor) {
        visitor.visitStructDefinition(this->dynamicCast<StructDefinition>());
    }

    Ast StructDefinition::getChildren() {
        Ast children = Construct::convertChildren<Value<>>(this->values);

        children.push_back(this->type);

        return children;
    }
}
