#pragma once

#include <vector>
#include <ionshared/misc/named.h>
#include <ionir/construct/instruction/alloca.h>
#include <ionir/construct/value.h>
#include <ionir/construct/construct.h>
#include <ionir/construct/type/struct.h>

namespace ionir {
    struct Pass;

    struct StructDefinition : Value<StructType> {
        static std::shared_ptr<StructDefinition> make(
            const std::shared_ptr<StructType>& type,
            const std::vector<std::shared_ptr<Value<>>>& values = {}
        ) noexcept;

        std::vector<std::shared_ptr<Value<>>> values;

        explicit StructDefinition(
            const std::shared_ptr<StructType>& type,
            std::vector<std::shared_ptr<Value<>>> values = {}
        );

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
