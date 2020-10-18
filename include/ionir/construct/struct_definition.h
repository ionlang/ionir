#pragma once

#include <vector>
#include <ionshared/misc/named.h>
#include "value.h"
#include "construct.h"
#include "struct.h"

namespace ionir {
    struct Pass;

    struct StructDefinition : Construct {
        std::shared_ptr<Struct> declaration;

        std::vector<std::shared_ptr<Value<>>> values;

        explicit StructDefinition(
            std::shared_ptr<Struct> declaration,
            std::vector<std::shared_ptr<Value<>>> values = {}
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
