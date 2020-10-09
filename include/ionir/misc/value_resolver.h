#pragma once

#include <optional>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Constant.h>
#include <ionir/construct/value/integer_literal.h>

namespace ionir {
    class ValueResolver {
    protected:
        llvm::Module* module;

    public:
        explicit ValueResolver(llvm::Module* module);

        [[nodiscard]] std::optional<llvm::Constant*> tryResolveInt(
            IntegerKind kind,
            long value,
            bool isSigned = true
        );
    };
}
