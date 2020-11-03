#include <ionir/misc/inst_builder.h>

namespace ionir {
    InstBuilder::InstBuilder(
        std::shared_ptr<BasicBlock> basicBlock
    ) noexcept :
        basicBlock(std::move(basicBlock)) {
        //
    }

    std::shared_ptr<AllocaInst> InstBuilder::createAlloca(
        const std::string& id,
        const std::shared_ptr<Type>& type
    ) {
        return this->setParentAndAppend(AllocaInst::make(type));
    }

    std::shared_ptr<StoreInst> InstBuilder::createStore(
        const std::shared_ptr<Value<>>& value,
        const std::shared_ptr<AllocaInst>& target
    ) {
        return this->setParentAndAppend(
            StoreInst::make(value, target)
        );
    }

    std::shared_ptr<BranchInst> InstBuilder::createBranch(
        const std::shared_ptr<Construct>& condition,
        const std::shared_ptr<BasicBlock>& consequentBasicBlock,
        const std::shared_ptr<BasicBlock>& alternativeBasicBlock
    ) {
        return this->setParentAndAppend(BranchInst::make(
            condition,
            consequentBasicBlock,
            alternativeBasicBlock
        ));
    }

    std::shared_ptr<ReturnInst> InstBuilder::createReturn(
        const ionshared::OptPtr<Value<>>& value
    ) {
        return this->setParentAndAppend(ReturnInst::make(value));
    }

    std::shared_ptr<CallInst> InstBuilder::createCall(
        const std::shared_ptr<Construct>& callee,
        const std::vector<std::shared_ptr<Construct>>& args
    ) {
        return this->setParentAndAppend(CallInst::make(callee, args));
    }

    std::shared_ptr<JumpInst> InstBuilder::createJump(
        const std::shared_ptr<BasicBlock>& basicBlock
    ) {
        return this->make<JumpInst>(basicBlock);
    }
}
