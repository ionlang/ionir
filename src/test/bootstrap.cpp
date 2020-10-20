#include <ionir/const/const.h>
#include <ionir/test/const.h>
#include <ionir/test/bootstrap.h>

namespace ionir::test::bootstrap {
    std::shared_ptr<ionshared::LlvmModule> llvmModule(const std::string& identifier) {
        return std::make_shared<ionshared::LlvmModule>(
            new llvm::Module(identifier, *new llvm::LLVMContext())
        );
    }

    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass(
        const std::shared_ptr<ionshared::LlvmModule>& module
    ) {
        std::shared_ptr<ionshared::SymbolTable<llvm::Module*>> modules =
            std::make_shared<ionshared::SymbolTable<llvm::Module*>>(
                ionshared::SymbolTable<llvm::Module*>({
                    {module->getId(), module->unwrap()}
                }
            )
        );

        // TODO: PassContext isn't associated with the calling scope/function in any way.
        std::shared_ptr<LlvmLoweringPass> llvmLoweringPass = std::make_shared<LlvmLoweringPass>(
            std::make_shared<ionshared::PassContext>(),
            modules
        );

        if (!llvmLoweringPass->setModuleBuffer(module->getId())) {
            throw std::runtime_error("Could not set active module buffer during bootstrap process");
        }

        return llvmLoweringPass;
    }

    std::shared_ptr<Function> emptyFunction(
        std::vector<std::shared_ptr<Instruction>> instructions
    ) {
        std::shared_ptr<VoidType> returnType = TypeFactory::typeVoid();

        // TODO: Consider support for module here.
        std::shared_ptr<Prototype> prototype = std::make_shared<Prototype>(
            test::constant::foobar,
            std::make_shared<Args>(),
            returnType,
            nullptr
        );

        std::shared_ptr<BasicBlock> entrySection = std::make_shared<BasicBlock>(BasicBlockOpts{
            nullptr,
            BasicBlockKind::Entry,
            Const::basicBlockEntryId,
            std::move(instructions)
        });

        // TODO: Fix mumbo-jumbo debugging code. -------------

        typedef PtrSymbolTable<BasicBlock> t;
        typedef ionshared::SymbolTable<std::shared_ptr<BasicBlock>> tt;

        auto t1 = std::map<std::string, std::shared_ptr<BasicBlock>>{
            {entrySection->name, entrySection}
        };

        t sections = std::make_shared<tt>(t1);

        // --------------------

        std::shared_ptr<Function> function = std::make_shared<Function>(prototype, nullptr);
        std::shared_ptr<FunctionBody> body = std::make_shared<FunctionBody>(function, sections);

        entrySection->parent = body;
        function->body = body;

        return function;
    }
}
