#include <ionir/construct/function_like.h>

#include <utility>

namespace ionir {
    FunctionLike::FunctionLike(
        std::shared_ptr<Prototype> prototype,
        std::vector<std::shared_ptr<BasicBlock>> basicBlocks
    ) noexcept :
        prototype(std::move(prototype)),
        basicBlocks(std::move(basicBlocks)){
        //
    }
}
