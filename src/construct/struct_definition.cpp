#include <ionir/passes/pass.h>

namespace ionir {
    StructDefinition::StructDefinition(
        std::shared_ptr<Struct> declaration,
        std::vector<std::shared_ptr<Value<>>> values
    ) noexcept :
        Value<Struct>(ValueKind::StructDefinition, std::move(declaration)),
        values(std::move(values)) {
        // TODO: What about the struct type (value type)?

        std::shared_ptr<Construct> self = this->nativeCast();

        for (const auto& value : values) {
            value->parent = self;
        }
    }

    void StructDefinition::accept(Pass& visitor) {
        visitor.visitStructDefinition(this->dynamicCast<StructDefinition>());
    }

    Ast StructDefinition::getChildrenNodes() {
        // TODO: What about the struct type (value type)?
        return Construct::convertChildren<Value<>>(this->values);
    }
}
