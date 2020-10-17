#pragma once

#include <vector>
#include <ionshared/misc/named.h>
#include "value.h"
#include "construct.h"
#include "struct.h"

namespace ionir {
    struct Pass;

    struct StructDefinition : Construct {
        ionshared::Ptr<Struct> declaration;

        std::vector<ionshared::Ptr<Value<>>> values;

        explicit StructDefinition(
            ionshared::Ptr<Struct> declaration,
            std::vector<ionshared::Ptr<Value<>>> values = {}
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
