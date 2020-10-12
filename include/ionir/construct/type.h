#pragma once

#include <string>
#include <ionshared/container/set.h>
#include <ionshared/misc/named.h>
#include "construct.h"

namespace ionir {
    class Pass;

    enum class TypeQualifier {
        Constant,

        Mutable,

        Reference,

        // TODO: With a linear pointer-array qualifier system, how can specify array of pointer or pointer to array?
        Pointer,

        Array
    };

    enum class TypeKind {
        UserDefined,

        Void,

        Integer,

        Decimal,

        String,

        Boolean,

        Pointer
    };

    typedef ionshared::Set<TypeQualifier> TypeQualifiers;

    struct Type : Construct, ionshared::Named {
        TypeKind typeKind;

        ionshared::Ptr<TypeQualifiers> qualifiers;

        /**
         * TODO: What if 'id' is atomic type yet kind is UserDefined?
         * Make 'Type' abstract and create UserDefinedType which
         * only takes 'id' (super class TypeKind::UserDefined).
         */
        explicit Type(
            std::string name,
            TypeKind kind = TypeKind::UserDefined,

            ionshared::Ptr<TypeQualifiers> qualifiers =
                std::make_shared<TypeQualifiers>()
        );

        [[nodiscard]] bool equals(const ionshared::Ptr<Construct> &other) override;

        [[nodiscard]] bool addQualifier(TypeQualifier qualifier) noexcept;

        [[nodiscard]] bool removeQualifier(TypeQualifier qualifier) noexcept;

        [[nodiscard]] bool hasQualifier(TypeQualifier qualifier) const;
    };
}
