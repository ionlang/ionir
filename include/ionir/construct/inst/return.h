#pragma once

#include <string>
#include <ionir/construct/type.h>
#include <ionir/construct/inst.h>

namespace ionir {
    struct Pass;

    struct ReturnInstOpts : InstOpts {
        ionshared::OptPtr<Value<>> value = std::nullopt;
    };

    struct ReturnInst : Instruction {
        ionshared::OptPtr<Value<>> value;

        explicit ReturnInst(const ReturnInstOpts& opts);

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool hasValue() noexcept;
    };
}
