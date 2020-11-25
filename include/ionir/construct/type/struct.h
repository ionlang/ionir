#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    typedef ionshared::PtrSymbolTable<Type> FieldSymbolTable;

    struct TypeStruct : Type {
        FieldSymbolTable fields;

        TypeStruct(std::string name, FieldSymbolTable fields) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildren() override;
    };
}
