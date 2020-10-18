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

    struct FunctionBody : ConstructWithParent<Function>, ScopeAnchor<BasicBlock> {
        explicit FunctionBody(
            std::shared_ptr<Function> parent,

            PtrSymbolTable<BasicBlock> symbolTable =
                ionshared::util::makePtrSymbolTable<BasicBlock>()
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool verify() override;

        [[nodiscard]] ionshared::OptPtr<BasicBlock> findEntryBasicBlock();

        [[nodiscard]] bool hasEntryBasicBlock();

        void insertBasicBlock(const std::shared_ptr<BasicBlock>& basicBlock);
    };
}
