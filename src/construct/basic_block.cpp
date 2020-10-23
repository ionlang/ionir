#include <ionir/misc/inst_builder.h>
#include <ionir/passes/pass.h>

namespace ionir {
    BasicBlock::BasicBlock(
        BasicBlockKind kind,
        std::vector<std::shared_ptr<Instruction>> instructions,
        PtrSymbolTable<Instruction> symbolTable
    ) noexcept :
        ConstructWithParent<FunctionBody>(ConstructKind::BasicBlock),
        ScopeAnchor<Instruction>(std::move(symbolTable)),
        basicBlockKind(kind),
        instructions(std::move(instructions)) {
        std::shared_ptr<Construct> self = this->nativeCast();

        for (const auto& instruction : instructions) {
            instruction->parent = self;
        }
    }

    void BasicBlock::accept(Pass& visitor) {
        // TODO: Casting fails. CRITICAL: This needs to work! LlvmCodegen's contextBuffer depends on it.
//        visitor.visitScopeAnchor(this->dynamicCast<ScopeAnchor<>>());
        visitor.visitBasicBlock(this->dynamicCast<BasicBlock>());
    }

    Ast BasicBlock::getChildrenNodes() {
        return Construct::convertChildren(this->instructions);
    }

    bool BasicBlock::verify() {
        return this->hasTerminalInst()
            && Construct::verify();
    }

    void BasicBlock::insertInst(uint32_t order, const std::shared_ptr<Instruction>& inst) {
        const uint32_t maxOrder = this->instructions.empty() ? 0 : this->instructions.size() - 1;

        if (order > maxOrder) {
            throw std::out_of_range("Order is larger than the size of elements in the vector");
        }

        this->instructions.insert(this->instructions.begin() + order, inst);

        // TODO: --- Repeated code below (appendInst). Simplify? Maybe create registerInst() function? ---

        const std::optional<std::string> id = util::findInstId(inst);

        // Instruction is named. Register it in the symbol table.
        if (id.has_value()) {
            this->getSymbolTable()->set(*id, inst);
        }
        // ----------------------------------------------------------
    }

    void BasicBlock::appendInst(const std::shared_ptr<Instruction>& inst) {
        this->instructions.push_back(inst);

        std::optional<std::string> id = util::findInstId(inst);

        // Instruction is named. Register it in the symbol table.
        if (id.has_value()) {
            this->getSymbolTable()->set(*id, inst);
        }
    }

    void BasicBlock::prependInst(const std::shared_ptr<Instruction>& inst) {
        this->insertInst(0, inst);
    }

    uint32_t BasicBlock::relocateInsts(BasicBlock &target, const uint32_t from) {
        uint32_t count = 0;

        for (uint32_t i = from; i < this->instructions.size(); i++) {
            target.instructions.push_back(this->instructions[i]);
            this->instructions.erase(this->instructions.begin() + i - 1);
            count++;
        }

        return count;
    }

    std::shared_ptr<BasicBlock> BasicBlock::split(uint32_t atOrder, std::string id) {
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

        std::shared_ptr<BasicBlock> newBasicBlock = Construct::make<BasicBlock>(
            this->forceGetUnboxedParent(),
            this->basicBlockKind,
            instructions
        );

        // TODO: Changed. Review.
        /**
         * Register the newly created basic block on the parent's
         * symbol table (parent is a function body).
         */
        this->forceGetUnboxedParent()->basicBlocks.push_back(newBasicBlock);

        return newBasicBlock;
    }

    std::shared_ptr<JumpInst> BasicBlock::link(const std::shared_ptr<BasicBlock>& basicBlock) {
        return this->createBuilder()->createJump(basicBlock);
    }

    std::optional<uint32_t> BasicBlock::locate(std::shared_ptr<Instruction> inst) const {
        return ionshared::util::locateInVector(this->instructions, std::move(inst));
    }

    ionshared::OptPtr<Instruction> BasicBlock::findInstByOrder(uint32_t order) const noexcept {
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

    ionshared::OptPtr<Instruction> BasicBlock::findTerminalInst() const noexcept {
        // TODO: There can only be a single return instruction.
        for (const auto& inst : this->instructions) {
            if (inst->isTerminal()) {
                return inst;
            }
        }

        return std::nullopt;
    }

    bool BasicBlock::hasTerminalInst() const noexcept {
        return ionshared::util::hasValue(this->findTerminalInst());
    }

    ionshared::OptPtr<Instruction> BasicBlock::findFirstInst() noexcept {
        if (!this->instructions.empty()) {
            return this->instructions.front();
        }

        return std::nullopt;
    }

    ionshared::OptPtr<Instruction> BasicBlock::findLastInst() noexcept {
        if (!this->instructions.empty()) {
            return this->instructions.back();
        }

        return std::nullopt;
    }
}
