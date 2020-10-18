#include <ionir/passes/type_system/borrow_check_pass.h>

namespace ionir {
    BorrowCheckPass::BorrowCheckPass(
        std::shared_ptr<ionshared::PassContext> context
    ) noexcept :
        Pass(std::move(context)) {
        //
    }
}
