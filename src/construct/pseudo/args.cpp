#include <ionir/construct/pseudo/args.h>

namespace ionir {
    Args::Args(
        std::shared_ptr<ionshared::SymbolTable<Arg>> items,
        bool isVariable
    ) noexcept :
        items(std::move(items)),
        isVariable(isVariable) {
        //
    }
}
