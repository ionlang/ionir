#include <ionir/passes/pass.h>

namespace ionir {
    Constructor::Constructor(
        std::vector<std::shared_ptr<BasicBlock>> basicBlocks
    ) noexcept :
        Construct(ConstructKind::Constructor),

        // TODO: Prototype.
        FunctionLike(std::move(prototype), std::move(basicBlocks)) {
        //
    }

    void Constructor::accept(Pass& visitor) {
        visitor.visitConstructor(this->dynamicCast<Constructor>());
    }
}
