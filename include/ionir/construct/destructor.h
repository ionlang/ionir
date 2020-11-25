#pragma once

#include <string>
#include <vector>
#include <ionir/construct/type/struct_type.h>
#include "function_like.h"

namespace ionir {
    struct Pass;

    // TODO: Shouldn't all this be on ionlang project?
    /**
     * A method-like construct which is automatically invoked when
     * struct instances are destroyed.
     */
    struct Destructor : Construct, FunctionLike, WithParent<TypeStruct> {
        explicit Destructor(
            std::vector<std::shared_ptr<BasicBlock>> basicBlocks
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
