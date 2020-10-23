#pragma once

#include <optional>
#include <vector>
#include <functional>
#include <ionir/misc/type_factory.h>
#include <ionir/tracking/scope_anchor.h>
#include "ionir/construct/pseudo/child_construct.h"
#include "basic_block.h"

namespace ionir {
    struct Pass;

    // TODO: Why have FunctionBody when you can just have Function with basicBlocks field?
    struct FunctionBody : ConstructWithParent<Function> {
        // TODO: Should be a set? But sets are weakly ordered (no concept of position, and .being() returns any element).
        std::vector<std::shared_ptr<BasicBlock>> basicBlocks;

        explicit FunctionBody(
            std::vector<std::shared_ptr<BasicBlock>> basicBlocks = {}
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool verify() override;
    };
}
