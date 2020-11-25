#include <ionir/passes/pass.h>

namespace ionir {
    InstJump::InstJump(
        std::shared_ptr<BasicBlock> basicBlockTarget
    ) noexcept :
        Instruction(InstructionKind::Jump),
        basicBlockTarget(std::move(basicBlockTarget)) {
        //
    }

    void InstJump::accept(Pass& visitor) {
        visitor.visitJumpInst(this->dynamicCast<InstJump>());
    }
}
