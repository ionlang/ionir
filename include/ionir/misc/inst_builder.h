#pragma once

#include <ionir/construct/instruction.h>
#include <ionir/construct/basic_block.h>
#include <ionir/construct/instruction/alloca.h>
#include <ionir/construct/instruction/store.h>
#include <ionir/construct/instruction/branch.h>
#include <ionir/construct/instruction/return.h>
#include <ionir/construct/instruction/call.h>
#include <ionir/construct/instruction/jump.h>
#include <ionir/construct/value/operation.h>

namespace ionir {
    struct InstBuilder {
        std::shared_ptr<BasicBlock> basicBlock;

        explicit InstBuilder(
            std::shared_ptr<BasicBlock> basicBlock
        ) noexcept;

        void appendInst(const std::shared_ptr<Instruction>& inst);

        template<class TInst, typename... TArgs>
            requires std::derived_from<TInst, Instruction>
        std::shared_ptr<TInst> make(TArgs... args) {
            // TODO: Ensure T inherits from Inst or derived.

            std::shared_ptr<TInst> inst = std::make_shared<TInst>(args...);

            this->appendInst(inst);

            return inst;
        }

        std::shared_ptr<AllocaInst> createAlloca(
            const std::string& id,
            std::shared_ptr<Type> type
        );

        std::shared_ptr<StoreInst> createStore(
            std::shared_ptr<Value<>> value,
            std::shared_ptr<AllocaInst> target
        );

        std::shared_ptr<BranchInst> createBranch(
            std::shared_ptr<Construct> condition,
            const std::shared_ptr<BasicBlock>& consequentBasicBlock,
            const std::shared_ptr<BasicBlock>& alternativeBasicBlock
        );

        std::shared_ptr<ReturnInst> createReturn(
            const ionshared::OptPtr<Value<>>& value = std::nullopt
        );

        std::shared_ptr<CallInst> createCall(
            const std::shared_ptr<Construct>& callee,
            const std::vector<std::shared_ptr<Construct>>& args = {}
        );

        std::shared_ptr<JumpInst> createJump(
            const std::shared_ptr<BasicBlock>& basicBlock
        );
    };
}
