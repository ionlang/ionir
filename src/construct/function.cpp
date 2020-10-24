#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<Function> Function::make(
        const std::shared_ptr<Prototype>& prototype,
        const std::vector<std::shared_ptr<BasicBlock>>& basicBlocks
    ) noexcept {
        std::shared_ptr<Function> result =
            std::make_shared<Function>(prototype, basicBlocks);

        prototype->parent = result;

        for (const auto& basicBlock : basicBlocks) {
            basicBlock->parent = result;
        }

        return result;
    }

    Function::Function(
        std::shared_ptr<Prototype> prototype,
        std::vector<std::shared_ptr<BasicBlock>> basicBlocks
    ) noexcept :
        Construct(ConstructKind::Function),
        prototype(std::move(prototype)),
        basicBlocks(std::move(basicBlocks)) {
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

    bool Function::verify() {
        return !this->basicBlocks.empty()
            && Construct::verify();
    }
}
