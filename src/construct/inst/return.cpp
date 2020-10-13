#include <ionir/passes/pass.h>

namespace ionir {
    ReturnInst::ReturnInst(const ReturnInstOpts& opts) :
        Inst(opts.parent, InstKind::Return),
        value(opts.value) {
        //
    }

    void ReturnInst::accept(Pass& visitor) {
        visitor.visitReturnInst(this->dynamicCast<ReturnInst>());
    }

    Ast ReturnInst::getChildrenNodes() {
        if (!this->hasValue()) {
            return Construct::getChildrenNodes();
        }

        return {
            *this->value
        };
    }

    bool ReturnInst::hasValue() noexcept {
        return ionshared::util::hasValue(this->value);
    }
}
