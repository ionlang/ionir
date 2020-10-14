#pragma once

#include <optional>
#include <string>
#include <vector>
#include <ionir/misc/type_factory.h>
#include <ionir/misc/util.h>
#include <ionir/tracking/scope_anchor.h>
#include <ionir/construct/pseudo/child_construct.h>
#include "pseudo/args.h"
#include "inst.h"
#include "type.h"

namespace ionir {
    struct Pass;

    struct FunctionBody;

    struct InstBuilder;

    struct JumpInst;

    enum class BasicBlockKind {
        /**
         * The entry basic block of a function body.
         */
        Entry,

        /**
         * A basic block defined by the user. Can be jumped to
         * using a goto instruction.
         */
        Label,

        /**
         * A basic block which forms part of a construct. Cannot be
         * directly accessed by the user.
         */
        Internal
    };

    struct BasicBlockOpts : util::ConstructParentMixin<FunctionBody> {
        const BasicBlockKind kind;

        const std::string id;

        std::vector<ionshared::Ptr<Inst>> insts = {};

        PtrSymbolTable<Inst> symbolTable = ionshared::util::makePtrSymbolTable<Inst>();
    };

    // TODO: Must be verified to contain a single terminal instruction at the end.
    struct BasicBlock : ConstructWithParent<FunctionBody>, ScopeAnchor<Inst>, ionshared::Named {
        const BasicBlockKind basicBlockKind;

        std::vector<ionshared::Ptr<Inst>> insts;

        explicit BasicBlock(const BasicBlockOpts& opts);

        void accept(Pass &visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool verify() override;

        void insertInst(uint32_t order, const ionshared::Ptr<Inst>& inst);

        /**
         * Inserts an instruction at the end of the instructions
         * list.
         */
        void appendInst(const ionshared::Ptr<Inst>& inst);

        /**
         * Inserts an instruction at the beginning of the instructions
         * list.
         */
        void prependInst(const ionshared::Ptr<Inst>& inst);

        uint32_t relocateInsts(BasicBlock& target, uint32_t from = 0);

        /**
         * Splits the local basic block, relocating all instructions
         * at and after the provided index to a new basic block with
         * the provided id, having the same parent as the local basic
         * block.
         */
        [[nodiscard]] ionshared::Ptr<BasicBlock> split(uint32_t atOrder, std::string id);

        /**
         * Create a jump instruction at the end of the local block
         * targeting the provided block using an instruction builder
         * instance. Returns the created jump instruction.
         */
        ionshared::Ptr<JumpInst> link(const ionshared::Ptr<BasicBlock>& basicBlock);

        /**
         * Attempt to find the index location of an instruction.
         * Returns std::nullopt if not found.
         */
        [[nodiscard]] std::optional<uint32_t> locate(ionshared::Ptr<Inst> inst);

        [[nodiscard]] ionshared::OptPtr<Inst> findInstByOrder(uint32_t order) const noexcept;

        [[nodiscard]] ionshared::Ptr<InstBuilder> createBuilder();

        [[nodiscard]] ionshared::OptPtr<Inst> findTerminalInst() const noexcept;

        [[nodiscard]] bool hasTerminalInst() const noexcept;

        [[nodiscard]] ionshared::OptPtr<Inst> findFirstInst() noexcept;

        [[nodiscard]] ionshared::OptPtr<Inst> findLastInst() noexcept;
    };
}
