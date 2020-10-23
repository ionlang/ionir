#include <ionir/construct/function.h>
#include <ionir/passes/pass.h>
#include <ionir/const/const.h>

namespace ionir {
    FunctionBody::FunctionBody(
        std::vector<std::shared_ptr<BasicBlock>> basicBlocks
    ) noexcept :
        ConstructWithParent<Function>(ConstructKind::FunctionBody),
        basicBlocks(std::move(basicBlocks)) {
        std::shared_ptr<Construct> self = this->nativeCast();

        for (const auto& basicBlock : this->basicBlocks) {
            basicBlock->parent = self;
        }
    }

    void FunctionBody::accept(Pass& visitor) {
        // TODO: CRITICAL: Cast error.
//        visitor.visitScopeAnchor(this->staticCast<ScopeAnchor<Section>>());
        visitor.visitFunctionBody(this->staticCast<FunctionBody>());
    }

    Ast FunctionBody::getChildrenNodes() {
        Ast children{};

        for (const auto& basicBlock : this->basicBlocks) {
            children.push_back(basicBlock);
        }

        return children;
    }

    bool FunctionBody::verify() {
        return !this->basicBlocks.empty()
            && Construct::verify();
    }
}
