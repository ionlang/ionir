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

    std::shared_ptr<Function> Bootstrap::function(const std::string& id) {
        std::shared_ptr<Module> module = Bootstrap::functionAst(id)[0]->dynamicCast<Module>();
        ionshared::OptPtr<Function> function = module->lookupFunction(id);

        // Ensure the function was retrieved, as a precaution.
        if (!ionshared::util::hasValue(function)) {
            throw std::runtime_error("Could not retrieve function from module's symbol table");
        }

        return *function;
    }

    std::shared_ptr<BasicBlock> Bootstrap::basicBlock(
        const std::shared_ptr<FunctionBody>& parent,
        BasicBlockKind basicBlockKind
    ) {
        return std::make_shared<BasicBlock>(
            parent,
            basicBlockKind
        );
    }
}
