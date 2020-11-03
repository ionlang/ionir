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
    class InstBuilder {
    private:
        template<typename T>
            requires std::derived_from<T, Instruction>
        std::shared_ptr<T> setParentAndAppend(
            std::shared_ptr<T> instruction
        ) noexcept {
            instruction->parent = this->basicBlock;
            this->basicBlock->instructions.push_back(instruction);

            return instruction;
        }

    public:
        std::shared_ptr<BasicBlock> basicBlock;

        explicit InstBuilder(
            std::shared_ptr<BasicBlock> basicBlock
        ) noexcept;

        /**
         * Construct a instruction, append it to the buffered basic,
         * block and set its parent.
         */
        template<class TInst, typename... TArgs>
            requires std::derived_from<TInst, Instruction>
        std::shared_ptr<TInst> make(TArgs... args) {
            std::shared_ptr<TInst> instruction =
                Construct::makeChild<TInst>(this->basicBlock, args...);

            this->basicBlock->instructions.push_back(instruction);

            return instruction;
        }

        std::shared_ptr<AllocaInst> createAlloca(
            const std::string& id,
            const std::shared_ptr<Type>& type
        );

        std::shared_ptr<StoreInst> createStore(
            const std::shared_ptr<Value<>>& value,
            const std::shared_ptr<AllocaInst>& target
        );

        std::shared_ptr<BranchInst> createBranch(
            const std::shared_ptr<Construct>& condition,
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
