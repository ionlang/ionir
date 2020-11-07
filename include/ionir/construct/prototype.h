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
        static std::shared_ptr<Prototype> make(
            const std::string& name,
            const std::shared_ptr<Args>& args,
            const std::shared_ptr<Type>& returnType
        ) noexcept;

        std::shared_ptr<Args> args;

        std::shared_ptr<Type> returnType;

        Prototype(
            std::string name,
            std::shared_ptr<Args> args,
            std::shared_ptr<Type> returnType
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool verify() override;

        bool isMain() const noexcept;
    };
}
