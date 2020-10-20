#pragma once

#include <string>

namespace ionir {
    struct Pass;

    struct Function;

    struct CallInst : Instruction {
        std::shared_ptr<Construct> callee;

        std::vector<std::shared_ptr<Construct>> args

        CallInst(
            const std::shared_ptr<BasicBlock>& parent,
            std::shared_ptr<Construct> callee,
            std::vector<std::shared_ptr<Construct>> args
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
