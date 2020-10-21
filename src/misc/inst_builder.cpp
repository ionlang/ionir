#include <ionir/misc/inst_builder.h>
#include <ionir/construct/function_body.h>

namespace ionir {
    InstBuilder::InstBuilder(
        std::shared_ptr<BasicBlock> basicBlock
    ) noexcept :
        basicBlock(std::move(basicBlock)) {
        //
    }

    void InstBuilder::appendInst(
        const std::shared_ptr<Instruction>& inst
    ) {
        this->basicBlock->instructions.push_back(inst);
    }

    std::shared_ptr<AllocaInst> InstBuilder::createAlloca(
        const std::string& id,
        std::shared_ptr<Type> type
    ) {
        return this->make<AllocaInst>(
            this->basicBlock,
            std::move(type)
        );
    }

    std::shared_ptr<StoreInst> InstBuilder::createStore(
        std::shared_ptr<Value<>> value,
        std::shared_ptr<AllocaInst> target
    ) {
        return this->make<StoreInst>(
            this->basicBlock,
            std::move(value),
            std::move(target)
        );
    }

    std::shared_ptr<BranchInst> InstBuilder::createBranch(
        std::shared_ptr<Construct> condition,
        const std::shared_ptr<BasicBlock>& consequentBasicBlock,
        const std::shared_ptr<BasicBlock>& alternativeBasicBlock
    ) {
        return this->make<BranchInst>(
            this->basicBlock,
            std::move(condition),
            consequentBasicBlock,
            alternativeBasicBlock
        );
    }

    std::shared_ptr<ReturnInst> InstBuilder::createReturn(
        const ionshared::OptPtr<Value<>>& value
    ) {
        return this->make<ReturnInst>(
            this->basicBlock,
            value
        );
    }

    std::shared_ptr<CallInst> InstBuilder::createCall(
        const std::shared_ptr<Construct>& callee,
        const std::vector<std::shared_ptr<Construct>>& args
    ) {
        return this->make<CallInst>(
            this->basicBlock,
            callee,
            args
        );
    }

    std::shared_ptr<JumpInst> InstBuilder::createJump(
        const std::shared_ptr<BasicBlock>& basicBlock
    ) {
        return this->make<JumpInst>(
            this->basicBlock,
            basicBlock
        );
    }
}
