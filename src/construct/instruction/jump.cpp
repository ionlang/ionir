#include <ionir/passes/pass.h>

namespace ionir {
    JumpInst::JumpInst(
        std::shared_ptr<BasicBlock> basicBlockTarget
    ) noexcept :
        Instruction(InstKind::Jump),
        basicBlockTarget(std::move(basicBlockTarget)) {
        //
    }

    void JumpInst::accept(Pass& visitor) {
        visitor.visitJumpInst(this->dynamicCast<JumpInst>());
    }
}
