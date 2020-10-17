#include <ionir/const/const.h>
#include <ionir/misc/value_resolver.h>

namespace ionir {
    ValueResolver::ValueResolver(llvm::Module *module) :
        module(module) {
        //
    }

    std::optional<llvm::Constant*> ValueResolver::tryResolveInt(IntegerKind kind, long value, bool isSigned) {
        std::optional<LlvmIntTypeResolver> resolver = Const::tryGetIntTypeResolver(kind);

        if (!resolver.has_value()) {
            return std::nullopt;
        }

        llvm::APInt apInt = llvm::APInt(value, isSigned);

        // TODO
        return std::nullopt;

        //return llvm::ConstantInt::get(resolver(this->context), apInt);
    }
}
