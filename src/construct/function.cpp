#include <ionir/passes/pass.h>

namespace ionir {
    Function::Function(
        std::shared_ptr<Prototype> prototype,
        std::vector<std::shared_ptr<BasicBlock>> basicBlocks
    ) noexcept :
        Construct(ConstructKind::Function),
        FunctionLike(std::move(prototype), std::move(basicBlocks)) {
        //
    }

    void Function::accept(Pass& visitor) {
        visitor.visitFunction(this->dynamicCast<Function>());
    }

    Ast Function::getChildrenNodes() {
        Ast children = Construct::convertChildren(this->basicBlocks);

        children.push_back(this->prototype);

        return children;
    }
}
