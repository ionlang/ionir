#pragma once

#include <ionshared/misc/named.h>
#include "construct.h"

namespace ionir {
    struct Pass;

    typedef ionshared::PtrSymbolTable<Type> Fields;

    struct Struct : Construct, ionshared::Named {
        Fields fields;

        Struct(std::string name, Fields fields) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool containsField(std::string name) const;

        [[nodiscard]] ionshared::OptPtr<Type> lookupField(std::string name);

        void setField(std::string name, ionshared::Ptr<Type> field);
    };
}
