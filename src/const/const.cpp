#include <ionir/const/const.h>

namespace ionir {
    ionshared::Map<IntegerKind, LlvmIntTypeResolver> Const::integerTypeResolverMap =
        ionshared::Map<IntegerKind, LlvmIntTypeResolver>();

    const std::string Const::nameManglerIonPrefix = "ionlang";

    const std::string Const::basicBlockEntryId = "entry";

    std::map<ConstructKind, std::string> Const::constructNames = {
        {ConstructKind::Type, "type"},
        {ConstructKind::FunctionBody, "function body"},
        {ConstructKind::Prototype, "prototype"},
        {ConstructKind::Function, "function"},
        {ConstructKind::Extern, "extern"},
        {ConstructKind::Inst, "instruction"},
        {ConstructKind::Global, "global"},
        {ConstructKind::BasicBlock, "basic block"},
        {ConstructKind::VariableRef, "variable reference"},
        {ConstructKind::VariableDecl, "variable declaration"},
        {ConstructKind::StructDecl, "struct declaration"},
        {ConstructKind::Module, "module"},
        {ConstructKind::Ref, "reference"},
        {ConstructKind::Value, "value"},
        {ConstructKind::RegisterAssign, "register assign"}
    };

    // TODO: Refactor.
    // Const::Const()
    // {
    //     // Register integer kind -> integer type resolvers.
    //     auto map = this->integerTypeResolverMap.unwrap();

    //     // TODO: Fix insertion error.
    //     // map.insert(IntegerKind::Int1, &llvm::IntegerType::getInt1Ty);
    //     // map.insert(IntegerKind::Int32, &llvm::IntegerType::getInt32Ty);
    //     // map.insert(IntegerKind::Int64, &llvm::IntegerType::getInt64Ty);
    //     // map.insert(IntegerKind::Int128, &llvm::IntegerType::getInt128Ty);
    // }

    std::optional<LlvmIntTypeResolver> Const::tryGetIntTypeResolver(IntegerKind kind) {
        return *Const::integerTypeResolverMap.lookup(kind);
    }

    std::optional<std::string> Const::getConstructKindName(ConstructKind constructKind) {
        if (ionshared::util::mapContains<ConstructKind, std::string>(Const::constructNames, constructKind)) {
            return Const::constructNames[constructKind];
        }

        return std::nullopt;
    }
}
