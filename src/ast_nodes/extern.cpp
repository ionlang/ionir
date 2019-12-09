#include "extern.h"
#include "passes/pass.h"

namespace ionir
{
Extern::Extern(std::shared_ptr<Prototype> prototype) : Node(NodeKind::Extern), prototype(prototype)
{
    //
}

Node *Extern::accept(Pass *visitor)
{
    return visitor->visitExtern(this);
}

std::shared_ptr<Prototype> Extern::getPrototype() const
{
    return this->prototype;
}
} // namespace ionir
