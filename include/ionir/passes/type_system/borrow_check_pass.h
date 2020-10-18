#pragma once

#include <ionir/passes/pass.h>

namespace ionir {
    // TODO
    struct BorrowCheckPass : Pass {
        IONSHARED_PASS_ID;

        explicit BorrowCheckPass(
            std::shared_ptr<ionshared::PassContext> context
        ) noexcept;
    };
}
