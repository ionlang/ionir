#pragma once

#include <ionir/construct/value.h>
#include <ionir/construct/pseudo/child_construct.h>

namespace ionir {
    struct Pass;

    struct BasicBlock;

    struct InstOpts {
        const ionshared::Ptr<BasicBlock> parent;
    };

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

    struct Inst : ConstructWithParent<BasicBlock> {
        const InstKind instKind;

        Inst(ionshared::Ptr<BasicBlock> parent, InstKind kind) noexcept;

        // TODO: Isn't this redundant?
        void accept(Pass& visitor) override = 0;

        [[nodiscard]] InstKind getInstKind() const noexcept;

        [[nodiscard]] bool isTerminal() const noexcept;

        [[nodiscard]] size_t getOrder();
    };
}
