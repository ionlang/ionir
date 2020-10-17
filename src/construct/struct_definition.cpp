#include <ionir/passes/pass.h>

namespace ionir {
    StructDefinition::StructDefinition(
        ionshared::Ptr<Struct> declaration,
        std::vector<ionshared::Ptr<Value<>>> values
    ) noexcept :
        Construct(ConstructKind::Struct),
        declaration(std::move(declaration)),
        values(std::move(values)) {
        //
    }

    void StructDefinition::accept(Pass& visitor) {
        visitor.visitStructDefinition(this->dynamicCast<StructDefinition>());
    }

    Ast StructDefinition::getChildrenNodes() {
        return Construct::convertChildren<Value<>>(this->values);
    }
}
