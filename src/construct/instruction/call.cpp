#include <ionir/passes/pass.h>

namespace ionir {
    CallInst::CallInst(
        std::shared_ptr<Construct> callee,
        std::vector<std::shared_ptr<Construct>> arguments
    ) noexcept :
        Instruction(InstKind::Call),
        callee(std::move(callee)),
        arguments(std::move(arguments)) {
        std::shared_ptr<Construct> self = this->nativeCast();

        for (const auto& argument : this->arguments) {
            argument->parent = self;
        }
    }

    void CallInst::accept(Pass& visitor) {
        visitor.visitCallInst(this->dynamicCast<CallInst>());
    }
}
