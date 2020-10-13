#pragma once

#include <string>

namespace ionir {
    struct Pass;

    struct Function;

    struct CallInstOpts : InstOpts {
        ionshared::Ptr<Construct> callee;

        std::vector<ionshared::Ptr<Construct>> args{};
    };

    struct CallInst : Inst {
        ionshared::Ptr<Construct> callee;

        std::vector<ionshared::Ptr<Construct>> args;

        explicit CallInst(const CallInstOpts& opts);

        void accept(Pass& visitor) override;
    };
}
