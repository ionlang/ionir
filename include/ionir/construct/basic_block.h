#pragma once

#include <optional>
#include <string>
#include <vector>
#include <ionir/misc/util.h>
#include <ionir/tracking/scope_anchor.h>
#include "pseudo/args.h"
#include "instruction.h"
#include "type.h"
#include "function_like.h"

namespace ionir {
    struct Pass;

    struct InstBuilder;

    struct InstJump;

    // TODO: Use other basic block kinds (and emit their corresponding names).
    enum struct BasicBlockKind {
        /**
         * A basic block containing intrinsic declarations used by the
         * compiler to keep track of the return value, among others.
         * Cannot be directly accessed by the user.
         */
        Declarations,

        /**
         * The first body basic block executed after declarations
         * are setup (if any). Cannot be directly accessed by the user.
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
        Intrinsic
    };

    // TODO: Parent must be function like (to account for method and function)
    // TODO: Must be verified to contain a single terminal instruction at the end.
    class BasicBlock :
        public Construct,
        public ScopeAnchor<Instruction>,
        public WithParent<FunctionLike> {
    private:
        /**
         * The basic block's instructions. Note that when inserting
         * instructions into a basic block, it should not be done
         * by directly accessing this instructions vector, because
         * once an instruction is associated with this basic block,
         * its parent must be updated, and this procedure is done
         * through a corresponding method such as the append or prepend
         * instruction methods.
         */
        std::vector<std::shared_ptr<Instruction>> instructions;

    public:
        const BasicBlockKind basicBlockKind;

        explicit BasicBlock(
            std::vector<std::shared_ptr<Instruction>> instructions = {},
            BasicBlockKind kind = BasicBlockKind::Intrinsic,

            PtrSymbolTable<Instruction> symbolTable =
                ionshared::util::makePtrSymbolTable<Instruction>()
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildren() override;

        // TODO: Must be const.
        std::vector<std::shared_ptr<Instruction>> getInstructions() const noexcept;

        void insertInstruction(
            uint32_t order,
            const std::shared_ptr<Instruction>& instruction
        );

        // TODO: Redundant? No symbol table is used anymore?
        /**
         * Inserts an instruction at the end of the instructions
         * list.
         */
        void appendInstruction(const std::shared_ptr<Instruction>& instruction);

        /**
         * Inserts an instruction at the beginning of the instructions
         * list.
         */
        void prependInstruction(const std::shared_ptr<Instruction>& instruction);

        uint32_t relocateInstructions(BasicBlock& target, uint32_t from = 0);

        /**
         * Splits the local basic block, relocating all instructions
         * at and after the provided index to a new basic block with
         * the provided id, having the same parent as the local basic
         * block.
         */
        [[nodiscard]] std::shared_ptr<BasicBlock> splitAt(uint32_t atOrder);

        /**
         * Create a jump instruction at the end of the local block
         * targeting the provided block using an instruction builder
         * instance. Returns the created jump instruction.
         */
        std::shared_ptr<InstJump> linkTo(const std::shared_ptr<BasicBlock>& basicBlock);

        /**
         * Attempt to find the index location of an instruction.
         * Returns std::nullopt if not found.
         */
        [[nodiscard]] std::optional<uint32_t> locateInstruction(
            std::shared_ptr<Instruction> instruction
        ) const;

        [[nodiscard]] ionshared::OptPtr<Instruction> findInstructionByOrder(
            uint32_t order
        ) const noexcept;

        [[nodiscard]] std::shared_ptr<InstBuilder> createBuilder();

        [[nodiscard]] ionshared::OptPtr<Instruction> findTerminalInstruction() const noexcept;

        [[nodiscard]] bool hasTerminalInst() const noexcept;

        [[nodiscard]] ionshared::OptPtr<Instruction> findFirstInstruction() noexcept;

        [[nodiscard]] ionshared::OptPtr<Instruction> findLastInstruction() noexcept;
    };
}
