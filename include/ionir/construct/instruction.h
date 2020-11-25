#pragma once

#include <ionir/construct/value.h>
#include <ionir/misc/with_parent.h>

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

    /**
     * A single instruction within a basic block.
     */
    struct Instruction : public Construct, public WithParent<BasicBlock> {
        friend class BasicBlock;

        const InstructionKind instKind;

        explicit Instruction(InstructionKind kind) noexcept;

        // TODO: Isn't this redundant?
        void accept(Pass& visitor) override = 0;

        [[nodiscard]] bool isTerminal() const noexcept;
    };
}
