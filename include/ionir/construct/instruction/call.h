#pragma once

#include <string>
#include <ionir/construct/instruction.h>

namespace ionir {
    struct Pass;

    struct Function;

    struct InstCall : Instruction {
        std::shared_ptr<Construct> callee;

        std::vector<std::shared_ptr<Construct>> arguments;

        explicit InstCall(
            std::shared_ptr<Construct> callee,
            std::vector<std::shared_ptr<Construct>> arguments = {}
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
