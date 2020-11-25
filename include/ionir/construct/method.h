#pragma once

#include <string>
#include <vector>
#include <ionshared/misc/named.h>
#include <ionir/construct/pseudo/args.h>
#include <ionir/construct/type/struct.h>
#include "type.h"
#include "function.h"
#include "function_like.h"

namespace ionir {
    struct Pass;

    // TODO: Shouldn't all this be on ionlang project?
    struct Method : Construct, FunctionLike, WithParent<TypeStruct> {
        const bool isOperatorOverload;

        Method(
            bool isOperatorOverload,
            std::shared_ptr<Prototype> prototype,
            std::vector<std::shared_ptr<BasicBlock>> basicBlocks
        ) noexcept;

        void accept(Pass& visitor) override;
    };
}
