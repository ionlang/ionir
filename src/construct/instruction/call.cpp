#include <ionir/passes/pass.h>

namespace ionir {
    CallInst::CallInst(
        const std::shared_ptr<BasicBlock>& parent,
        std::shared_ptr<Construct> callee,
        std::vector<std::shared_ptr<Construct>> args
    ) noexcept :
        Instruction(parent, InstKind::Call),
        callee(std::move(callee)),
        args(std::move(args)) {
        //
    }

    void CallInst::accept(Pass& visitor) {
        visitor.visitCallInst(this->dynamicCast<CallInst>());
    }
}
