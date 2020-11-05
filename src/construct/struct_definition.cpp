#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<StructDefinition> StructDefinition::make(
        const std::shared_ptr<StructType>& type,
        const std::vector<std::shared_ptr<Value<>>>& values
    ) noexcept {
        std::shared_ptr<StructDefinition> result =
            std::make_shared<StructDefinition>(type, values);

        type->parent = result;

        for (const auto& value : values) {
            value->parent = result;
        }

        return result;
    }

    StructDefinition::StructDefinition(
        const std::shared_ptr<StructType>& type,
        std::vector<std::shared_ptr<Value<>>> values
    ) :
        Value<StructType>(
            ValueKind::StructDefinition,
            type
        ),

        values(std::move(values)) {
        //
    }

    void StructDefinition::accept(Pass& visitor) {
        visitor.visitStructDefinition(this->dynamicCast<StructDefinition>());
    }

    Ast StructDefinition::getChildrenNodes() {
        Ast children = Construct::convertChildren<Value<>>(this->values);

        children.push_back(this->type);

        return children;
    }
}
