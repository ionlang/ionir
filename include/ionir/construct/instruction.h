#pragma once

#include <ionir/construct/value.h>
#include <ionir/construct/pseudo/child_construct.h>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    enum struct InstKind {
        Alloca,

        Store,

        Call,

        Return,

        Break,

        Branch,

        Compare,

        Jump
    };

    struct Instruction : ConstructWithParent<BasicBlock> {
        const InstKind instKind;

        Instruction(std::shared_ptr<BasicBlock> parent, InstKind kind) noexcept;

        // TODO: Isn't this redundant?
        void accept(Pass& visitor) override = 0;

        [[nodiscard]] bool isTerminal() const noexcept;

        [[nodiscard]] size_t fetchOrder();
    };
}
