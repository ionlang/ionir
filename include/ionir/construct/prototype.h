#pragma once

#include <string>
#include <vector>
#include <ionshared/misc/named.h>
#include <ionir/construct/pseudo/args.h>
#include "ionir/construct/pseudo/child_construct.h"
#include "type.h"
#include "module.h"

namespace ionir {
    struct Pass;

    struct Prototype : ConstructWithParent<Module>, ionshared::Named {
        std::string id;

        ionshared::Ptr<Args> args;

        ionshared::Ptr<Type> returnType;

        Prototype(
            std::string id,
            ionshared::Ptr<Args> args,
            ionshared::Ptr<Type> returnType,
            ionshared::Ptr<Module> parent
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool verify() override;
    };
}
