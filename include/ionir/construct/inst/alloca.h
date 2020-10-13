#pragma once

#include <ionir/construct/type.h>
#include <ionir/construct/inst.h>

namespace ionir {
    struct BasicBlock;

    struct AllocaInstOpts : InstOpts {
        ionshared::Ptr<Type> type;
    };

    struct AllocaInst : Inst {
        ionshared::Ptr<Type> type;

        // TODO: Missing support for array-type allocas.
        explicit AllocaInst(const AllocaInstOpts& opts);

        void accept(Pass& visitor) override;
    };
}
