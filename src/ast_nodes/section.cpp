#include "passes/pass.h"
#include "code_gen/node_kind.h"
#include "section.h"

namespace ionir
{
Section::Section(SectionKind kind, std::string id, std::vector<Inst *> insts)
    : Node(NodeKind::Section), kind(kind), id(id), insts(insts)
{
    //
}

Node *Section::accept(Pass *visitor)
{
    return visitor->visitSection(this);
}

SectionKind Section::getKind() const
{
    return this->kind;
}

std::string Section::getId() const
{
    return this->id;
}

std::vector<Inst *> Section::getInsts() const
{
    return this->insts;
}
} // namespace ionir