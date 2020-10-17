#pragma once

#include <ionir/construct/inst.h>
#include <ionir/construct/basic_block.h>
#include <ionir/construct/inst/alloca.h>
#include <ionir/construct/inst/store.h>
#include <ionir/construct/inst/branch.h>
#include <ionir/construct/inst/return.h>
#include <ionir/construct/inst/call.h>
#include <ionir/construct/inst/jump.h>
#include <ionir/construct/value/operation.h>

namespace ionir {
    struct InstBuilder {
        ionshared::Ptr<BasicBlock> basicBlock;

        explicit InstBuilder(
            ionshared::Ptr<BasicBlock> basicBlock
        ) noexcept;

        void appendInst(const ionshared::Ptr<Inst>& inst);

        template<class TInst, typename... TArgs>
            requires std::derived_from<TInst, Inst>
        ionshared::Ptr<TInst> make(TArgs... args) {
            // TODO: Ensure T inherits from Inst or derived.

            ionshared::Ptr<TInst> inst = std::make_shared<TInst>(args...);

            this->appendInst(inst);

            return inst;
        }

        ionshared::Ptr<AllocaInst> createAlloca(
            const std::string& id,
            ionshared::Ptr<Type> type
        );

        ionshared::Ptr<StoreInst> createStore(
            ionshared::Ptr<Value<>> value,
            ionshared::Ptr<AllocaInst> target
        );

        ionshared::Ptr<BranchInst> createBranch(
            ionshared::Ptr<Construct> condition,
            const ionshared::Ptr<BasicBlock>& consequentBasicBlock,
            const ionshared::Ptr<BasicBlock>& alternativeBasicBlock
        );

        ionshared::Ptr<ReturnInst> createReturn(
            const ionshared::OptPtr<Value<>>& value = std::nullopt
        );

        ionshared::Ptr<CallInst> createCall(
            const ionshared::Ptr<Construct>& callee,
            const std::vector<ionshared::Ptr<Construct>>& args = {}
        );

        ionshared::Ptr<JumpInst> createJump(
            const ionshared::Ptr<BasicBlock>& basicBlock
        );
    };
}
