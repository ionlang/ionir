#include <ionir/misc/inst_builder.h>
#include <ionir/passes/pass.h>

namespace ionir {
    BasicBlock::BasicBlock(
        std::vector<std::shared_ptr<Instruction>> instructions,
        BasicBlockKind kind,
        PtrSymbolTable<Instruction> symbolTable
    ) noexcept :
        Construct(ConstructKind::BasicBlock),
        ScopeAnchor<Instruction>(std::move(symbolTable)),
        basicBlockKind(kind),
        instructions(std::move(instructions)) {
        //
    }

    void BasicBlock::accept(Pass& visitor) {
        // TODO: Casting fails. CRITICAL: This needs to work! LlvmCodegen's contextBuffer depends on it.
//        visitor.visitScopeAnchor(this->dynamicCast<ScopeAnchor<>>());
        visitor.visitBasicBlock(this->dynamicCast<BasicBlock>());
    }

    Ast BasicBlock::getChildren() {
        return Construct::convertChildren(this->instructions);
    }

    std::vector<std::shared_ptr<Instruction>> BasicBlock::getInstructions() const noexcept {
        return this->instructions;
//        return std::vector<const std::shared_ptr<Instruction>>(
//            this->instructions.cbegin(),
//            this->instructions.cend()
//        );
    }

    void BasicBlock::insertInstruction(uint32_t order, const std::shared_ptr<Instruction>& instruction) {
        const uint32_t maxOrder = this->instructions.empty() ? 0 : this->instructions.size() - 1;

        if (order > maxOrder) {
            throw std::out_of_range("Order is larger than the size of elements in the vector");
        }

        this->instructions.insert(this->instructions.begin() + order, instruction);

        // TODO: --- Repeated code below (appendInst). Simplify? Maybe create registerInst() function? ---

        const std::optional<std::string> id = util::findInstId(instruction);

        // Instruction is named. Register it in the symbol table.
        if (id.has_value()) {
            this->getSymbolTable()->set(*id, instruction);
        }
        // ----------------------------------------------------------
    }

    void BasicBlock::appendInstruction(const std::shared_ptr<Instruction>& instruction) {
        this->instructions.push_back(instruction);

        std::optional<std::string> id = util::findInstId(instruction);

        // Instruction is named. Register it in the symbol table.
        if (id.has_value()) {
            this->getSymbolTable()->set(*id, instruction);
        }
    }

    void BasicBlock::prependInstruction(const std::shared_ptr<Instruction>& instruction) {
        this->insertInstruction(0, instruction);
    }

    uint32_t BasicBlock::relocateInstructions(BasicBlock& target, uint32_t from) {
        uint32_t count = 0;

        for (uint32_t i = from; i < this->instructions.size(); i++) {
            target.instructions.push_back(this->instructions[i]);
            this->instructions.erase(this->instructions.begin() + i - 1);
            count++;
        }

        return count;
    }

    std::shared_ptr<BasicBlock> BasicBlock::splitAt(uint32_t atOrder) {
        // TODO: If insts are empty, atOrder parameter is ignored (useless). Address that.
        // TODO: Symbol table is not being relocated/split.

        if (!this->instructions.empty() && (atOrder < 0 || atOrder > this->instructions.size() - 1)) {
            throw std::out_of_range("Provided order is outsize of bounds");
        }

        std::vector<std::shared_ptr<Instruction>> instructions{};

        if (!this->instructions.empty()) {
            auto from = this->instructions.begin() + atOrder;
            auto to = this->instructions.end();

            instructions = std::vector<std::shared_ptr<Instruction>>(from, to);

            // Erase the instructions from the local basic block.
            this->instructions.erase(from, to);
        }

        std::shared_ptr<BasicBlock> newBasicBlock = std::make_shared<BasicBlock>(
            instructions,
            this->basicBlockKind
        );

        // TODO: Changed/commented out. Review.
        /**
         * Register the newly created basic block on the parent's
         * symbol table (parent is a function body).
         */
//        this->forceGetUnboxedParent()->basicBlocks.push_back(newBasicBlock);

        return newBasicBlock;
    }

    std::shared_ptr<InstJump> BasicBlock::linkTo(const std::shared_ptr<BasicBlock>& basicBlock) {
        return this->createBuilder()->makeJump(basicBlock);
    }

    std::optional<uint32_t> BasicBlock::locateInstruction(std::shared_ptr<Instruction> instruction) const {
        return ionshared::util::locateInVector(this->instructions, std::move(instruction));
    }

    ionshared::OptPtr<Instruction> BasicBlock::findInstructionByOrder(uint32_t order) const noexcept {
        /**
         * Provided order is larger than the amount of elements in the
         * insts vector. No need to continue, return std::nullopt.
         */
        if (this->instructions.empty() || this->instructions.size() < order + 1) {
            return std::nullopt;
        }

        return this->instructions[order];
    }

    std::shared_ptr<InstBuilder> BasicBlock::createBuilder() {
        return std::make_shared<InstBuilder>(this->dynamicCast<BasicBlock>());
    }

    ionshared::OptPtr<Instruction> BasicBlock::findTerminalInstruction() const noexcept {
        // TODO: There can only be a single return instruction.
        for (const auto& instruction : this->instructions) {
            if (instruction->isTerminal()) {
                return instruction;
            }
        }

        return std::nullopt;
    }

    bool BasicBlock::hasTerminalInst() const noexcept {
        return ionshared::util::hasValue(this->findTerminalInstruction());
    }

    ionshared::OptPtr<Instruction> BasicBlock::findFirstInstruction() noexcept {
        if (!this->instructions.empty()) {
            return this->instructions.front();
        }

        return std::nullopt;
    }

    ionshared::OptPtr<Instruction> BasicBlock::findLastInstruction() noexcept {
        if (!this->instructions.empty()) {
            return this->instructions.back();
        }

        return std::nullopt;
    }
}
