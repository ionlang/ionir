#pragma once

#include <string>
#include <vector>
#include <ionir/construct/type/struct.h>
#include "function_like.h"

namespace ionir {
    struct Pass;

    // TODO: Shouldn't all this be on ionlang project?
    struct Destructor : Construct, FunctionLike, WithParent<TypeStruct> {
        explicit Destructor(
            std::vector<std::shared_ptr<BasicBlock>> basicBlocks
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
