#include "passes/pass.h"
#include "return.h"

namespace ionir
{
// TODO: Finish init. implementation.
ReturnInst::ReturnInst(std::optional<Ptr<Value>> value) : Inst(InstKind::Return), value(value)
{
    //
}

void ReturnInst::accept(Pass *visitor)
{
    visitor->visitReturnInst(this->cast<ReturnInst>());
}

std::optional<Ptr<Value>> ReturnInst::getValue() const
{
    return this->value;
}
} // namespace ionir