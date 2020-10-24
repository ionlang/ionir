#pragma once

#include <optional>
#include <string>
#include "value.h"
#include "construct.h"
#include "type.h"

namespace ionir {
    struct Pass;

    struct Global : Construct, ionshared::Named {
        static std::shared_ptr<Global> make(
            const std::shared_ptr<Type>& type,
            const std::string& id,
            ionshared::OptPtr<Value<>> value = std::nullopt
        ) noexcept;

        std::shared_ptr<Type> type;

        ionshared::OptPtr<Value<>> value;

        Global(
            std::shared_ptr<Type> type,
            std::string id,
            ionshared::OptPtr<Value<>> value = std::nullopt
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;
    };
}
