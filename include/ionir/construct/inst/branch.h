#pragma once

#include <optional>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    struct BranchInstOpts : InstOpts {
        std::shared_ptr<Construct> condition;

        std::shared_ptr<BasicBlock> consequentBasicBlock;

        std::shared_ptr<BasicBlock> alternativeBasicBlock;
    };

    struct BranchInst : Instruction {
        std::shared_ptr<Construct> condition;

        std::shared_ptr<BasicBlock> consequentBasicBlock;

        std::shared_ptr<BasicBlock> alternativeBasicBlock;

        explicit BranchInst(const BranchInstOpts& opts);

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
