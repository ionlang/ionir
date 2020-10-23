#pragma once

#include <string>
#include <ionir/construct/instruction.h>

namespace ionir {
    struct Pass;

    struct Function;

    struct CallInst : Instruction {
        std::shared_ptr<Construct> callee;

        std::vector<std::shared_ptr<Construct>> arguments;

        explicit CallInst(
            std::shared_ptr<Construct> callee,
            std::vector<std::shared_ptr<Construct>> arguments = {}
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
