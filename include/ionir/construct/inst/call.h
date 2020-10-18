#pragma once

#include <string>

namespace ionir {
    struct Pass;

    struct Function;

    struct CallInstOpts : InstOpts {
        std::shared_ptr<Construct> callee;

        std::vector<std::shared_ptr<Construct>> args{};
    };

    struct CallInst : Instruction {
        std::shared_ptr<Construct> callee;

        std::vector<std::shared_ptr<Construct>> args;

        explicit CallInst(const CallInstOpts& opts);

        void accept(Pass& visitor) override;
    };
}
