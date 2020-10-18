#include <ionir/passes/pass.h>

namespace ionir {
    BranchInst::BranchInst(const BranchInstOpts& opts) :
        Instruction(opts.parent, InstKind::Branch),
        condition(opts.condition),
        consequentBasicBlock(opts.consequentBasicBlock),
        alternativeBasicBlock(opts.alternativeBasicBlock) {
        //
    }

    void BranchInst::accept(Pass& visitor) {
        visitor.visitBranchInst(this->dynamicCast<BranchInst>());
    }

    Ast BranchInst::getChildrenNodes() {
        // TODO: What about condition?

        return {
            this->consequentBasicBlock,
            this->alternativeBasicBlock
        };
    }
}
