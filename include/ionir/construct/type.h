#pragma once

#include <string>
#include <ionshared/container/set.h>
#include <ionshared/misc/named.h>
#include "construct.h"

namespace ionir {
    struct Pass;

    enum struct TypeQualifier {
        Constant,

        Mutable,

        Reference,

        // TODO: With a linear pointer-array qualifier system, how can specify array of pointer or pointer to array?
        Pointer,

        Array
    };

    enum struct TypeKind {
        UserDefined,

        Void,

        Integer,

        Decimal,

        String,

        Boolean,

        Pointer
    };

    typedef ionshared::Set<TypeQualifier> TypeQualifiers;

    struct Type : Construct {
        const TypeKind typeKind;

        const std::optional<std::string> name;

        ionshared::Ptr<TypeQualifiers> qualifiers;

        /**
         * TODO: What if 'id' is atomic type yet kind is UserDefined?
         * Make 'Type' abstract and create UserDefinedType which
         * only takes 'id' (super class TypeKind::UserDefined).
         */
        explicit Type(
            TypeKind kind = TypeKind::UserDefined,
            std::optional<std::string> name = std::nullopt,

            ionshared::Ptr<TypeQualifiers> qualifiers =
                std::make_shared<TypeQualifiers>()
        ) noexcept;

        [[nodiscard]] bool equals(const ionshared::Ptr<Construct>& other) override;
    };
}
