#pragma once

#include <optional>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    struct JumpInstOpts : InstOpts {
        ionshared::Ptr<BasicBlock> basicBlockTarget;
    };

    struct JumpInst : Inst {
        ionshared::Ptr<BasicBlock> basicBlockTarget;

        explicit JumpInst(const JumpInstOpts& opts);

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
