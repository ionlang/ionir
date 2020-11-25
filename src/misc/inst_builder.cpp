#include <ionir/misc/inst_builder.h>

namespace ionir {
    InstBuilder::InstBuilder(
        std::shared_ptr<BasicBlock> basicBlock
    ) noexcept :
        basicBlock(std::move(basicBlock)) {
        //
    }

    std::shared_ptr<InstAlloca> InstBuilder::makeAlloca(
        const std::string& id,
        const std::shared_ptr<Type>& type
    ) {
        return this->appendInstruction(
            std::make_shared<InstAlloca>(type)
        );
    }

    std::shared_ptr<InstStore> InstBuilder::makeStore(
        const std::shared_ptr<Value<>>& value,
        const std::shared_ptr<InstAlloca>& target
    ) {
        return this->appendInstruction(
            std::make_shared<InstStore>(value, target)
        );
    }

    std::shared_ptr<InstBranch> InstBuilder::makeBranch(
        const std::shared_ptr<Construct>& condition,
        const std::shared_ptr<BasicBlock>& consequentBasicBlock,
        const std::shared_ptr<BasicBlock>& alternativeBasicBlock
    ) {
        return this->appendInstruction(std::make_shared<InstBranch>(
            condition,
            consequentBasicBlock,
            alternativeBasicBlock
        ));
    }

    std::shared_ptr<InstReturn> InstBuilder::makeReturn(
        const ionshared::OptPtr<Value<>>& value
    ) {
        return this->appendInstruction(std::make_shared<InstReturn>(value));
    }

    std::shared_ptr<InstCall> InstBuilder::makeCall(
        const std::shared_ptr<Construct>& callee,
        const std::vector<std::shared_ptr<Construct>>& args
    ) {
        return this->appendInstruction(std::make_shared<InstCall>(callee, args));
    }

    std::shared_ptr<InstJump> InstBuilder::makeJump(
        const std::shared_ptr<BasicBlock>& basicBlock
    ) {
        return this->appendInstruction(std::make_shared<InstJump>(basicBlock));
    }
}
