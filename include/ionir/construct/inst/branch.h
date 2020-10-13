#pragma once

#include <optional>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    struct BranchInstOpts : InstOpts {
        ionshared::Ptr<Construct> condition;

        ionshared::Ptr<BasicBlock> consequentBasicBlock;

        ionshared::Ptr<BasicBlock> alternativeBasicBlock;
    };

    struct BranchInst : Inst {
        ionshared::Ptr<Construct> condition;

        ionshared::Ptr<BasicBlock> consequentBasicBlock;

        ionshared::Ptr<BasicBlock> alternativeBasicBlock;

        explicit BranchInst(const BranchInstOpts& opts);

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
