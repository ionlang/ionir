#pragma once

#include <ionir/construct/type.h>
#include <ionir/construct/construct.h>

namespace ionir {
    struct NameOfIntrinsic : Construct {
        std::shared_ptr<Type> type;

        explicit NameOfIntrinsic(std::shared_ptr<Type> type);

        void accept(Pass& visitor) override;

        // TODO: Should the type be a child?
    };
}
