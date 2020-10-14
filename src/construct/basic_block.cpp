#include <ionir/misc/inst_builder.h>
#include <ionir/passes/pass.h>

namespace ionir {
    BasicBlock::BasicBlock(const BasicBlockOpts &opts) :
        ConstructWithParent(opts.parent, ConstructKind::BasicBlock),
        ScopeAnchor<Inst>(opts.symbolTable),
        Named{opts.id},
        basicBlockKind(opts.kind),
        insts(opts.insts) {
        //
    }

    void BasicBlock::accept(Pass &visitor) {
        // TODO: Casting fails. CRITICAL: This needs to work! LlvmCodegen's contextBuffer depends on it.
//        visitor.visitScopeAnchor(this->dynamicCast<ScopeAnchor<>>());
        visitor.visitBasicBlock(this->dynamicCast<BasicBlock>());
    }

    Ast BasicBlock::getChildrenNodes() {
        return Construct::convertChildren(this->insts);
    }

    bool BasicBlock::verify() {
        return this->hasTerminalInst()
            && ionshared::util::hasValue(this->parent)
            && Construct::verify(*this->parent)
            && Construct::verify();
    }

    void BasicBlock::insertInst(uint32_t order, const ionshared::Ptr<Inst> &inst) {
        const uint32_t maxOrder = this->insts.empty() ? 0 : this->insts.size() - 1;

        if (order > maxOrder) {
            throw std::out_of_range("Order is larger than the size of elements in the vector");
        }

        this->insts.insert(this->insts.begin() + order, inst);

        // TODO: --- Repeated code below (appendInst). Simplify? Maybe create registerInst() function? ---

        const std::optional<std::string> id = util::findInstId(inst);

        // Instruction is named. Register it in the symbol table.
        if (id.has_value()) {
            this->getSymbolTable()->set(*id, inst);
        }
        // ----------------------------------------------------------
    }

    void BasicBlock::appendInst(const ionshared::Ptr<Inst> &inst) {
        this->insts.push_back(inst);

        std::optional<std::string> id = util::findInstId(inst);

        // Instruction is named. Register it in the symbol table.
        if (id.has_value()) {
            this->getSymbolTable()->set(*id, inst);
        }
    }

    void BasicBlock::prependInst(const ionshared::Ptr<Inst> &inst) {
        this->insertInst(0, inst);
    }

    uint32_t BasicBlock::relocateInsts(BasicBlock &target, const uint32_t from) {
        uint32_t count = 0;

        for (uint32_t i = from; i < this->insts.size(); i++) {
            target.insts.push_back(this->insts[i]);
            this->insts.erase(this->insts.begin() + i - 1);
            count++;
        }

        return count;
    }

    ionshared::Ptr<BasicBlock> BasicBlock::split(uint32_t atOrder, std::string id) {
        // TODO: If insts are empty, atOrder parameter is ignored (useless). Address that.
        // TODO: Symbol table is not being relocated/split.

        if (!this->insts.empty() && (atOrder < 0 || atOrder > this->insts.size() - 1)) {
            throw std::out_of_range("Provided order is outsize of bounds");
        }

        std::vector<ionshared::Ptr<Inst>> insts = {};

        if (!this->insts.empty()) {
            auto from = this->insts.begin() + atOrder;
            auto to = this->insts.end();

            insts = std::vector<ionshared::Ptr<Inst>>(from, to);

            // Erase the instructions from the local basic block.
            this->insts.erase(from, to);
        }

        ionshared::Ptr<BasicBlock> newBasicBlock = std::make_shared<BasicBlock>(BasicBlockOpts{
            this->getUnboxedParent(),
            this->basicBlockKind,
            std::move(id),
            insts
        });

        /**
         * Register the newly created basic block on the parent's
         * symbol table (parent is a function body).
         */
        this->getUnboxedParent()->insertBasicBlock(newBasicBlock);

        return newBasicBlock;
    }

    ionshared::Ptr<JumpInst> BasicBlock::link(const ionshared::Ptr<BasicBlock> &basicBlock) {
        return this->createBuilder()->createJump(basicBlock);
    }

    std::optional<uint32_t> BasicBlock::locate(ionshared::Ptr<Inst> inst) {
        return ionshared::util::locateInVector(this->insts, std::move(inst));
    }

    ionshared::OptPtr<Inst> BasicBlock::findInstByOrder(uint32_t order) const noexcept {
        /**
         * Provided order is larger than the amount of elements in the
         * insts vector. No need to continue, return std::nullopt.
         */
        if (this->insts.empty() || this->insts.size() < order + 1) {
            return std::nullopt;
        }

        return this->insts[order];
    }

    ionshared::Ptr<InstBuilder> BasicBlock::createBuilder() {
        return std::make_shared<InstBuilder>(this->dynamicCast<BasicBlock>());
    }

    ionshared::OptPtr<Inst> BasicBlock::findTerminalInst() const noexcept {
        // TODO: There can only be a single return instruction.
        for (const auto &inst : this->insts) {
            if (inst->isTerminal()) {
                return inst;
            }
        }

        return std::nullopt;
    }

    bool BasicBlock::hasTerminalInst() const noexcept {
        return ionshared::util::hasValue(this->findTerminalInst());
    }

    ionshared::OptPtr<Inst> BasicBlock::findFirstInst() noexcept {
        if (!this->insts.empty()) {
            return this->insts.front();
        }

        return std::nullopt;
    }

    ionshared::OptPtr<Inst> BasicBlock::findLastInst() noexcept {
        if (!this->insts.empty()) {
            return this->insts.back();
        }

        return std::nullopt;
    }
}
