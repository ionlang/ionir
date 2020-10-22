#pragma once

#include <string>
#include <map>
#include <optional>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <ionshared/container/map.h>
#include <ionir/construct/value/integer_literal.h>

namespace ionir {
    typedef llvm::IntegerType* (llvm::IntegerType::*LlvmIntTypeResolver)(llvm::LLVMContext& context);

    class Const {
    private:
        static ionshared::Map<IntegerKind, LlvmIntTypeResolver> integerTypeResolverMap;

    public:
        static const std::string basicBlockInternalPrefix;

        static const std::string basicBlockEntryId;

        [[nodiscard]] static std::optional<LlvmIntTypeResolver> tryGetIntTypeResolver(IntegerKind kind);

        static std::map<ConstructKind, std::string> constructNames;

        [[nodiscard]] static std::optional<std::string> getConstructKindName(ConstructKind constructKind);
    };
}
