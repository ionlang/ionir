#include <ionir/passes/pass.h>

namespace ionir {
    InstCall::InstCall(
        std::shared_ptr<Construct> callee,
        std::vector<std::shared_ptr<Construct>> arguments
    ) noexcept :
        Instruction(InstructionKind::Call),
        callee(std::move(callee)),
        arguments(std::move(arguments)) {
        //
    }

    void InstCall::accept(Pass& visitor) {
        visitor.visitCallInst(this->dynamicCast<InstCall>());
    }
}
