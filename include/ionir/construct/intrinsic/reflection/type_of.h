#pragma once

#include <ionir/construct/value.h>
#include <ionir/construct/construct.h>

namespace ionir {
    struct TypeOfIntrinsic : Construct {
        std::shared_ptr<Value<>> value;

        explicit TypeOfIntrinsic(std::shared_ptr<Value<>> value);

        void accept(Pass& visitor) override;

        // TODO: Should the value be a child?
    };
}
