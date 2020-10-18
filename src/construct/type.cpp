#include <ionir/passes/pass.h>

namespace ionir {
    Type::Type(
        TypeKind kind,
        std::optional<std::string> name,
        std::shared_ptr<ionshared::Set<TypeQualifier>> qualifiers
    ) noexcept :
        Construct(ConstructKind::Type),
        typeKind(kind),
        name(std::move(name)),
        qualifiers(std::move(qualifiers)) {
        //
    }

    bool Type::equals(const std::shared_ptr<Construct>& other) {
        if (other->constructKind != ConstructKind::Type) {
            return false;
        }

        std::shared_ptr<Type> otherType = other->dynamicCast<Type>();

        return otherType->typeKind == this->typeKind
            && otherType->name == this->name;
    }
}
