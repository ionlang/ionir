#pragma once

#include <ionir/construct/type.h>
#include <ionir/construct/prototype.h>

namespace ionir {
    struct Pass;

    enum struct ValueKind {
        Integer,

        Character,

        String,

        Boolean
    };

    template<typename T = Type>
//        requires std::derived_from<T, Type>
    struct Value : Construct {
        const ValueKind valueKind;

        ionshared::Ptr<T> type;

        Value(ValueKind kind, ionshared::Ptr<T> type) noexcept :
            Construct(ConstructKind::Value),
            valueKind(kind),
            type(type) {
            //
        }
    };
}
