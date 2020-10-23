#pragma once

#include <ionir/construct/type.h>
#include <ionir/construct/prototype.h>

namespace ionir {
    struct Pass;

    enum struct ValueKind {
        Integer,

        Character,

        String,

        Boolean,

        Operation,

        StructDefinition
    };

    template<typename T = Type>
//        requires std::derived_from<T, Type>
    struct Value : Construct {
        const ValueKind valueKind;

        std::shared_ptr<T> type;

        Value(ValueKind kind, std::shared_ptr<T> type) noexcept :
            Construct(ConstructKind::Value),
            valueKind(kind),
            type(type) {
            //
        }

        [[nodiscard]] Ast getChildrenNodes() override {
            return {
                this->type
            };
        }

        std::shared_ptr<Value<>> flatten() {
            return this->staticCast<Value<>>();
        }
    };
}
