#include <ionir/construct/pseudo/args.h>

namespace ionir {
    Args::Args(ionshared::Ptr<ionshared::SymbolTable<Arg>> items, bool isVariable) :
        items(std::move(items)),
        isVariable(isVariable) {
        //
    }
}
