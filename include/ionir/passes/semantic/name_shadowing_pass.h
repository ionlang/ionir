#pragma once

#include <ionir/misc/helpers.h>
#include <ionir/passes/pass.h>

namespace ionir {
    /**
     * Performs semantic analysis on the AST,
     * ensuring that variables, functions, and
     * classes are not re-defined nor shadowed.
     */
    class NameShadowingPass : public Pass {
    public:
        void visitInst(Ptr<Inst> node) override;
    };
}
