#include <ionir/misc/inst_builder.h>
#include <ionir/construct/function_body.h>

namespace ionir {
    InstBuilder::InstBuilder(ionshared::Ptr<BasicBlock> basicBlock) noexcept :
        basicBlock(std::move(basicBlock)) {
        //
    }

    void InstBuilder::appendInst(const ionshared::Ptr<Inst>& inst) {
        this->basicBlock->insts.push_back(inst);
    }

    ionshared::Ptr<AllocaInst> InstBuilder::createAlloca(
        const std::string& id,
        ionshared::Ptr<Type> type
    ) {
        return this->make<AllocaInst>(AllocaInstOpts{
            this->basicBlock,
            std::move(type)
        });
    }

    ionshared::Ptr<StoreInst> InstBuilder::createStore(
        ionshared::Ptr<Value<>> value,
        ionshared::Ptr<AllocaInst> target
    ) {
        return this->make<StoreInst>(StoreInstOpts{
            this->basicBlock,
            std::move(value),
            std::move(target)
        });
    }

    ionshared::Ptr<BranchInst> InstBuilder::createBranch(
        ionshared::Ptr<Construct> condition,
        const ionshared::Ptr<BasicBlock>& consequentBasicBlock,
        const ionshared::Ptr<BasicBlock>& alternativeBasicBlock
    ) {
        return this->make<BranchInst>(BranchInstOpts{
            this->basicBlock,
            std::move(condition),
            consequentBasicBlock,
            alternativeBasicBlock
        });
    }

    ionshared::Ptr<ReturnInst> InstBuilder::createReturn(
        const ionshared::OptPtr<Value<>>& value
    ) {
        return this->make<ReturnInst, ReturnInstOpts>(ReturnInstOpts{
            this->basicBlock,
            value
        });
    }

    ionshared::Ptr<CallInst> InstBuilder::createCall(
        const ionshared::Ptr<Construct>& callee,
        const std::vector<ionshared::Ptr<Construct>>& args
    ) {
        return this->make<CallInst>(CallInstOpts{
            this->basicBlock,
            callee,
            args
        });
    }

    ionshared::Ptr<JumpInst> InstBuilder::createJump(
        const ionshared::Ptr<BasicBlock>& basicBlock
    ) {
        return this->make<JumpInst>(JumpInstOpts{
            this->basicBlock,
            basicBlock
        });
    }
}
