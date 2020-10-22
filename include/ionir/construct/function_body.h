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

    struct FunctionBody : ConstructWithParent<Function> {
        // TODO: Should be a set? But sets are weakly ordered (no concept of position, and .being() returns any element).
        std::vector<std::shared_ptr<BasicBlock>> basicBlocks;

        explicit FunctionBody(
            std::shared_ptr<Function> parent,
            std::vector<std::shared_ptr<BasicBlock>> basicBlocks = {}
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool verify() override;

        void insertBasicBlock(const std::shared_ptr<BasicBlock>& basicBlock);
    };
}
