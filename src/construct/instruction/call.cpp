#include <ionir/passes/pass.h>

namespace ionir {
    std::shared_ptr<CallInst> CallInst::make(
        const std::shared_ptr<Construct>& callee,
        const std::vector<std::shared_ptr<Construct>>& arguments
    ) noexcept {
        std::shared_ptr<CallInst> result =
            std::make_shared<CallInst>(callee, arguments);

        for (const auto& argument : arguments) {
            argument->parent = result;
        }

        return result;
    }

    CallInst::CallInst(
        std::shared_ptr<Construct> callee,
        std::vector<std::shared_ptr<Construct>> arguments
    ) noexcept :
        Instruction(InstructionKind::Call),
        callee(std::move(callee)),
        arguments(std::move(arguments)) {
        //
    }

    void CallInst::accept(Pass& visitor) {
        visitor.visitCallInst(this->dynamicCast<CallInst>());
    }
}
