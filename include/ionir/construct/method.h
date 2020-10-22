#pragma once

#include <string>
#include <vector>
#include <ionshared/misc/named.h>
#include <ionir/construct/pseudo/args.h>
#include "ionir/construct/pseudo/child_construct.h"
#include "type.h"
#include "ionir/construct/type/struct.h"
#include "function.h"

namespace ionir {
    struct Pass;

    struct Method : ConstructWithParent<Struct> {
        std::shared_ptr<Function> function;

        Method(
            std::string id,
            std::shared_ptr<Args> args,
            std::shared_ptr<Type> returnType,
            std::shared_ptr<Module> parent
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool verify() override;
    };
}
