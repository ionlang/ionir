#include <ionir/passes/pass.h>

namespace ionir {
    StructDecl::StructDecl() :
        Construct(ConstructKind::StructDecl) {
        //
    }

    void StructDecl::accept(Pass& visitor) {
        return visitor.visitStructDecl(this->dynamicCast<StructDecl>());
    }
}
