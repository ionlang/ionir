#include <ionir/misc/inst_builder.h>
#include <ionir/misc/util.h>
#include <ionir/passes/pass.h>

namespace ionir {
    BasicBlock::BasicBlock(BasicBlockOpts opts)
        : ChildConstruct(opts.parent, ConstructKind::BasicBlock), ScopeAnchor<Inst>(), kind(opts.kind), Named(opts.id), insts(opts.insts) {
        //
    }

    void BasicBlock::accept(Pass &visitor) {
        visitor.visitScopeAnchor(this->dynamicCast<ScopeAnchor<>>());
        visitor.visitBasicBlock(this->dynamicCast<BasicBlock>());
    }

    Ast BasicBlock::getChildNodes() const {
        return Construct::convertChildren(this->insts);
    }

    BasicBlockKind BasicBlock::getKind() const noexcept {
        return this->kind;
    }

    std::vector<Ptr<Inst>> &BasicBlock::getInsts() noexcept {
        return this->insts;
    }

    // TODO: SymbolTable must be re-populated after changing insts vector.
    void BasicBlock::setInsts(std::vector<Ptr<Inst>> insts) {
        this->insts = insts;
    }

    void BasicBlock::insertInst(Ptr<Inst> inst) {
        this->insts.push_back(inst);

        std::optional<std::string> id = Util::getInstId(inst);

        // Instruction is named. Register it in the symbol table.
        if (id.has_value()) {
            this->getSymbolTable()->insert(*id, inst);
        }
    }

    uint32_t BasicBlock::relocateInsts(BasicBlock &target, const uint32_t from) {
        uint32_t count = 0;

        for (uint32_t i = from; i < this->insts.size(); i++) {
            target.getInsts().push_back(this->insts[i]);
            this->insts.erase(this->insts.begin() + i - 1);
            count++;
        }

        return count;
    }

    std::optional<uint32_t> BasicBlock::locate(Ptr<Inst> inst) const {
        return Util::locateInVector<Ptr<Inst>>(this->insts, inst);
    }

    Ptr<InstBuilder> BasicBlock::createBuilder() {
        return std::make_shared<InstBuilder>(this->dynamicCast<BasicBlock>());
    }

    OptPtr<Inst> BasicBlock::findTerminalInst() const {
        for (const auto inst : this->insts) {
            if (inst->isTerminal()) {
                return inst;
            }
        }

        return std::nullopt;
    }
}