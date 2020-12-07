#pragma once

#include <ionir/tracking/symbol_table.h>
#include <ionir/misc/with_parent.h>
#include "construct.h"
#include "prototype.h"
#include "basic_block.h"
#include "function_like.h"

namespace ionir {
    struct Pass;

    /**
     * A global function within a module.
     */
    struct Function : Construct, FunctionLike, WithParent<Module> {
        Function(
            std::shared_ptr<Prototype> prototype,
            std::vector<std::shared_ptr<BasicBlock>> basicBlocks
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildren() override;

        [[nodiscard]] std::optional<std::string> getName() override;
    };
}
