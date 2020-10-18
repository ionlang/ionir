#pragma once

#include <ionir/construct/type.h>
#include <ionir/construct/inst.h>

namespace ionir {
    struct BasicBlock;

    struct AllocaInstOpts : InstOpts {
        std::shared_ptr<Type> type;
    };

    struct AllocaInst : Instruction {
        std::shared_ptr<Type> type;

        // TODO: Missing support for array-type allocas.
        explicit AllocaInst(const AllocaInstOpts& opts);

        void accept(Pass& visitor) override;
    };
}
