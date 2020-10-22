#include <ionir/construct/function.h>
#include <ionir/passes/pass.h>
#include <ionir/const/const.h>

namespace ionir {
    FunctionBody::FunctionBody(
        std::shared_ptr<Function> parent,
        std::vector<std::shared_ptr<BasicBlock>> basicBlocks
    ) noexcept :
        ConstructWithParent(std::move(parent),
        ConstructKind::FunctionBody),
        basicBlocks(std::move(basicBlocks)) {
        //
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

    void FunctionBody::insertBasicBlock(const std::shared_ptr<BasicBlock>& basicBlock) {
        this->basicBlocks.push_back(basicBlock);

        // TODO: Consider making symbol table read-only so this is controlled.
        // Update the basic block's parent.
        basicBlock->parent = this->nativeCast();
    }
}
