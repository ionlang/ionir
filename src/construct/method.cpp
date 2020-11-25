#include <ionir/passes/pass.h>

namespace ionir {
    Method::Method(
        bool isOperatorOverload,
        std::shared_ptr<Prototype> prototype,
        std::vector<std::shared_ptr<BasicBlock>> basicBlocks
    ) noexcept :
        Construct(ConstructKind::Method),
        FunctionLike(std::move(prototype), std::move(basicBlocks)),
        isOperatorOverload(isOperatorOverload) {
        //
    }

    void Method::accept(Pass& visitor) {
        visitor.visitMethod(this->dynamicCast<Method>());
    }
}
