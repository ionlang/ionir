#pragma once

#include <optional>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    struct JumpInstOpts : InstOpts {
        std::shared_ptr<BasicBlock> basicBlockTarget;
    };

    struct JumpInst : Instruction {
        std::shared_ptr<BasicBlock> basicBlockTarget;

        explicit JumpInst(const JumpInstOpts& opts);

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
