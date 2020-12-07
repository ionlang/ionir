#pragma once

#include <string>
#include <vector>
#include <ionshared/misc/named.h>
#include <ionir/construct/pseudo/args.h>
#include "type.h"
#include "module.h"

namespace ionir {
    struct Pass;

    struct Prototype : Construct, ionshared::Named {
        std::shared_ptr<Args> args;

        std::shared_ptr<Type> returnType;

        Prototype(
            std::string name,
            std::shared_ptr<Args> args,
            std::shared_ptr<Type> returnType
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildren() override;

        [[nodiscard]] std::optional<std::string> getName() override;

        bool isMain() const noexcept;
    };
}
