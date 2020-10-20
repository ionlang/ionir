#include <ionir/passes/pass.h>

namespace ionir {
    JumpInst::JumpInst(
        const std::shared_ptr<BasicBlock>& parent,
        std::shared_ptr<BasicBlock> basicBlockTarget
    ) noexcept :
        Instruction(parent, InstKind::Jump),
        basicBlockTarget(std::move(basicBlockTarget)) {
        //
    }

    void JumpInst::accept(Pass& visitor) {
        visitor.visitJumpInst(this->dynamicCast<JumpInst>());
    }

    Ast JumpInst::getChildrenNodes() {
        // TODO: Should have no children, function bodies should be LINEAR! BRANCH defines inline block! WRONG!
        return {
            this->basicBlockTarget
        };
    }
}
