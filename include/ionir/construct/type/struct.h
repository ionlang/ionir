#pragma once

#include <ionir/construct/type.h>

namespace ionir {
    struct Pass;

    typedef ionshared::PtrSymbolTable<Type> Fields;

    struct StructType : Type {
        static std::shared_ptr<StructType> make(
            const std::string& name,
            const Fields& fields
        ) noexcept;

        Fields fields;

        StructType(std::string name, Fields fields) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
