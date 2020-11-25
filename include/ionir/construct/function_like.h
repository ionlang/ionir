#pragma once

#include "construct.h"

namespace ionir {
    struct Pass;

    struct Prototype;

    struct BasicBlock;

    struct FunctionLike {
        std::shared_ptr<Prototype> prototype;

        std::vector<std::shared_ptr<BasicBlock>> basicBlocks;

        FunctionLike(
            std::shared_ptr<Prototype> prototype,
            std::vector<std::shared_ptr<BasicBlock>> basicBlocks
        ) noexcept;
    };
}
