#pragma once

#include <optional>
#include <ionir/misc/helpers.h>
#include <ionir/construct/pseudo/ref.h>

namespace ionir {
    class Pass;

    class BasicBlock;

    struct BranchInstOpts : InstOpts {
        ionshared::Ptr<Construct> condition;

        PtrRef<BasicBlock> blockRef;

        PtrRef<BasicBlock> otherwiseRef;
    };

    class BranchInst : public Inst {
    private:
        ionshared::Ptr<Construct> condition;

        PtrRef<BasicBlock> blockRef;

        PtrRef<BasicBlock> otherwiseBlockRef;

    public:
        explicit BranchInst(const BranchInstOpts &opts);

        void accept(Pass &visitor) override;

        Ast getChildNodes() override;

        ionshared::Ptr<Construct> getCondition() const;

        void setCondition(ionshared::Ptr<Construct> condition);

        PtrRef<BasicBlock> getBlockRef() const;

        void setBlockRef(PtrRef<BasicBlock> blockRef);

        PtrRef<BasicBlock> getOtherwiseBlockRef() const;

        void setOtherwiseBlockRef(PtrRef<BasicBlock> otherwiseBlockRef);
    };
}
