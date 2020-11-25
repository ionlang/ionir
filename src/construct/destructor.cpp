#include <ionir/passes/pass.h>

namespace ionir {
    Destructor::Destructor(
        std::vector<std::shared_ptr<BasicBlock>> basicBlocks
    ) noexcept :
        Construct(ConstructKind::Destructor),

        // TODO: Prototype.
        FunctionLike(std::move(prototype), std::move(basicBlocks)) {
        //
    }

    void Destructor::accept(Pass& visitor) {
        visitor.visitDestructor(this->dynamicCast<Destructor>());
    }
}
