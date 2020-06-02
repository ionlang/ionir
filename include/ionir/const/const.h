#pragma once

#include <string>
#include <map>
#include <optional>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <ionir/construct/value/integer.h>
#include <ionir/lexical/token_kind.h>
#include <ionir/container/map.h>

namespace ionir {
    typedef llvm::IntegerType *(llvm::IntegerType::*LlvmIntTypeResolver)(llvm::LLVMContext &context);

    class Const {
    private:
        static Map<IntegerKind, LlvmIntTypeResolver> integerTypeResolverMap;

    public:
        static const std::string basicBlockInternalPrefix;

        static const std::string basicBlockEntryId;

        static std::optional<LlvmIntTypeResolver> tryGetIntTypeResolver(IntegerKind kind);

        static std::map<TokenKind, int> operatorPrecedence;

        static std::map<ConstructKind, std::string> constructNames;

        static std::map<TokenKind, IntegerKind> tokenKindToIntegerKind;

        static std::optional<std::string> getConstructKindName(ConstructKind constructKind);

        static std::optional<IntegerKind> getIntegerKind(TokenKind tokenKind);
    };
}
