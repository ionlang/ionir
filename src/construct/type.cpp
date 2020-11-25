#include <ionir/passes/pass.h>

namespace ionir {
    Type::Type(
        TypeKind kind,
        std::string name,
        std::shared_ptr<ionshared::Set<TypeQualifier>> qualifiers
    ) noexcept :
        Construct(ConstructKind::Type),
        typeKind(kind),
        typeName(std::move(name)),
        qualifiers(std::move(qualifiers)) {
        //
    }

    bool Type::equals(const std::shared_ptr<Construct>& other) {
        if (other->constructKind != ConstructKind::Type) {
            return false;
        }

        std::shared_ptr<Type> otherType = other->dynamicCast<Type>();

        return otherType->typeKind == this->typeKind
            && otherType->typeName == this->typeName;
    }

    bool Type::isSameAs(
        const std::shared_ptr<Type>& other
    ) {
        if (this->typeKind != other->typeKind) {
            return false;
        }

        switch (this->typeKind) {
            case TypeKind::Integer: {
                std::shared_ptr<TypeInteger> localIntegerType =
                    this->dynamicCast<TypeInteger>();

                std::shared_ptr<TypeInteger> otherIntegerType =
                    other->dynamicCast<TypeInteger>();

                return localIntegerType->integerKind == otherIntegerType->integerKind;
            }

                // TODO: Decimal types as well (copy integer code basically).

            case TypeKind::Struct: {
                std::shared_ptr<TypeStruct> localStructType =
                    this->dynamicCast<TypeStruct>();

                std::shared_ptr<TypeStruct> otherStructType =
                    other->dynamicCast<TypeStruct>();

                bool nameOrArgumentCountMismatch = localStructType->typeName != otherStructType->typeName
                    || localStructType->fields->getSize() != otherStructType->fields->getSize();

                if (nameOrArgumentCountMismatch) {
                    return false;
                }

                auto localStructFieldsNativeMap = localStructType->fields->unwrap();

                for (const auto& [name, type] : localStructFieldsNativeMap) {
                    // TODO: CRITICAL: If the same struct is referenced, it might cause infinite loop. Research.
                    bool fieldNameOrTypeMismatch = !otherStructType->fields->contains(name)
                        || !otherStructType->fields->lookup(name)->get()->isSameAs(type);

                    if (fieldNameOrTypeMismatch) {
                        return false;
                    }
                }

                return true;
            }

            default: {
                throw std::runtime_error("Unknown type kind");
            }
        }
    }
}
