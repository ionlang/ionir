#pragma once

#include <ionir/construct/inst.h>
#include <ionir/construct/value.h>
#include <ionir/construct/pseudo/child_construct.h>

namespace ionir {
    struct Pass;

    struct StoreInstOpts : InstOpts {
        std::shared_ptr<Value<>> value;

        std::shared_ptr<AllocaInst> target;
    };

    struct StoreInst : Instruction {
        std::shared_ptr<Value<>> value;

        std::shared_ptr<AllocaInst> target;

        explicit StoreInst(const StoreInstOpts& opts);

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
