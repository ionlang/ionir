#pragma once

#include <string>
#include <ionshared/misc/util.h>
#include <ionir/tracking/scope_anchor.h>
#include <ionir/tracking/context.h>
#include <ionir/misc/identifiable.h>
#include "type.h"
#include "construct.h"

namespace ionir {
    struct Pass;

    struct Function;

    struct Module : Construct, Identifiable {
        std::shared_ptr<Context> context;

        explicit Module(
            std::shared_ptr<Identifier> identifier,
            std::shared_ptr<Context> context = std::make_shared<Context>()
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildren() override;

        // TODO: What about externs/globals/classes/structs? ------------
        bool insertFunction(const std::shared_ptr<Function>& function);

        [[nodiscard]] ionshared::OptPtr<Function> lookupFunction(std::string name);
        // --------------------------------------------------------------

        /**
         * Attempt to move context scopes (including global scope) from
         * this module to another. Will return false if there are conflicts
         * (keys which already exist on the target) or true otherwise. No
         * changes will be committed if any conflict occurs (or the process
         * fails).
         */
        bool mergeInto(const std::shared_ptr<Module>& module);
    };
}
