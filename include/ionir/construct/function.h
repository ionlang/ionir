#pragma once

#include <ionir/tracking/symbol_table.h>
#include "construct.h"
#include "prototype.h"
#include "basic_block.h"

namespace ionir {
    struct Pass;

    struct Function : Construct {
        static std::shared_ptr<Function> make(
            const std::shared_ptr<Prototype>& prototype,
            const std::vector<std::shared_ptr<BasicBlock>>& basicBlocks
        ) noexcept;

        std::shared_ptr<Prototype> prototype;

        std::vector<std::shared_ptr<BasicBlock>> basicBlocks;

        Function(
            std::shared_ptr<Prototype> prototype,
            std::vector<std::shared_ptr<BasicBlock>> basicBlocks
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool verify() override;
    };
}
