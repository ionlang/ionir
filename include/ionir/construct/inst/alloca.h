#pragma once

#include <ionir/construct/type.h>
#include <ionir/construct/inst_yield.h>
#include <ionir/construct/inst.h>

namespace ionir {
    class BasicBlock;

    struct AllocaInstOpts : InstYieldOpts {
        ionshared::Ptr<Type> type;
    };

    struct AllocaInst : Inst, InstYield {
        ionshared::Ptr<Type> type;

        // TODO: Missing support for array-type allocas.
        explicit AllocaInst(const AllocaInstOpts& opts);

        void accept(Pass& visitor) override;
    };
}
