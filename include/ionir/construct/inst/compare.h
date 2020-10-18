#pragma once

#include <ionir/construct/inst.h>

namespace ionir {
    struct Pass;

    struct Function;

    struct CompareInstOpts : InstOpts {
        // TODO
    };

    struct CompareInst : Instruction {
        explicit CompareInst(const CompareInstOpts& opts);

        void accept(Pass& visitor) override;
    };
}
