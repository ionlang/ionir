#pragma once

#include <ionir/construct/value.h>
#include <ionir/construct/pseudo/child_construct.h>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    enum struct InstructionKind {
        Alloca,

        Store,

        Call,

        Return,

        Break,

        Branch,

        Compare,

        Jump,

        Cast
    };

    struct Instruction : ConstructWithParent<BasicBlock> {
        const InstructionKind instKind;

        explicit Instruction(InstructionKind kind) noexcept;

        // TODO: Isn't this redundant?
        void accept(Pass& visitor) override = 0;

        [[nodiscard]] bool isTerminal() const noexcept;

        [[nodiscard]] size_t fetchOrder();
    };
}
