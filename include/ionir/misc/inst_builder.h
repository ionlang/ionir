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
        std::shared_ptr<T> appendInstruction(
            std::shared_ptr<T> instruction
        ) noexcept {
            this->basicBlock->instructions.push_back(instruction);

            return instruction;
        }

    public:
        std::shared_ptr<BasicBlock> basicBlock;

        explicit InstBuilder(
            std::shared_ptr<BasicBlock> basicBlock
        ) noexcept;

        std::shared_ptr<InstAlloca> makeAlloca(
            const std::string& id,
            const std::shared_ptr<Type>& type
        );

        std::shared_ptr<InstStore> makeStore(
            const std::shared_ptr<Value<>>& value,
            const std::shared_ptr<InstAlloca>& target
        );

        std::shared_ptr<InstBranch> makeBranch(
            const std::shared_ptr<Construct>& condition,
            const std::shared_ptr<BasicBlock>& consequentBasicBlock,
            const std::shared_ptr<BasicBlock>& alternativeBasicBlock
        );

        std::shared_ptr<InstReturn> makeReturn(
            const ionshared::OptPtr<Value<>>& value = std::nullopt
        );

        std::shared_ptr<InstCall> makeCall(
            const std::shared_ptr<Construct>& callee,
            const std::vector<std::shared_ptr<Construct>>& args = {}
        );

        std::shared_ptr<InstJump> makeJump(
            const std::shared_ptr<BasicBlock>& basicBlock
        );
    };
}
