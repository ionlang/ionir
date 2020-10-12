#include <ionir/construct/inst_yield.h>

namespace ionir {
    InstYield::InstYield(std::string yieldName) noexcept :
        yieldName(std::move(yieldName)) {
        //
    }
}
