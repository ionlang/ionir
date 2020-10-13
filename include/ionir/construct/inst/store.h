#pragma once

#include <ionir/construct/inst.h>
#include <ionir/construct/value.h>
#include <ionir/construct/pseudo/child_construct.h>

namespace ionir {
    struct Pass;

    struct StoreInstOpts : InstOpts {
        ionshared::Ptr<Value<>> value;

        ionshared::Ptr<AllocaInst> target;
    };

    struct StoreInst : Inst {
        ionshared::Ptr<Value<>> value;

        ionshared::Ptr<AllocaInst> target;

        explicit StoreInst(const StoreInstOpts& opts);

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
