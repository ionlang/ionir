#include <ionir/const/const_name.h>
#include <ionir/syntax/ast_builder.h>
#include <ionir/misc/bootstrap.h>

namespace ionir {
    Ast Bootstrap::functionAst(const std::string& id) {
        return AstBuilder()
            .module(ConstName::anonymous)
            .function(id)
            .make();
    }

    std::shared_ptr<Function> Bootstrap::function(const std::string& name) {
        std::shared_ptr<Module> module = Bootstrap::functionAst(name)[0]->dynamicCast<Module>();
        std::optional<std::shared_ptr<Function>> function = module->lookup<Function>(name);

        // Ensure the function was retrieved, as a precaution.
        if (!ionshared::util::hasValue(function)) {
            throw std::runtime_error("Could not retrieve function from module's symbol table");
        }

        return *function;
    }

    std::shared_ptr<BasicBlock> Bootstrap::basicBlock(
        const std::shared_ptr<Function>& parent,
        BasicBlockKind basicBlockKind
    ) {
        // TODO: Parent function/block/expr.
        return std::make_shared<BasicBlock>(
            std::vector<std::shared_ptr<Instruction>>{},
            basicBlockKind
        );
    }
}
