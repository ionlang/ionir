#pragma once

#include "construct.h"
#include "prototype.h"
#include "basic_block.h"

namespace ionir {
    struct Pass;

    struct FunctionLike {
        std::shared_ptr<Prototype> prototype;

        std::vector<std::shared_ptr<BasicBlock>> basicBlocks;

        FunctionLike(
            std::shared_ptr<Prototype> prototype,
            std::vector<std::shared_ptr<BasicBlock>> basicBlocks
        ) noexcept;
    };
}
