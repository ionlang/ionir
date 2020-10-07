#include <ionir/const/const.h>
#include <ionir/test/const.h>
#include <ionir/test/bootstrap.h>

namespace ionir::test::bootstrap {
    ionshared::Ptr<ionshared::LlvmModule> llvmModule(const std::string &identifier) {
        llvm::LLVMContext *llvmContext = new llvm::LLVMContext();
        llvm::Module *llvmModule = new llvm::Module("test", *llvmContext);

        return std::make_shared<ionshared::LlvmModule>(llvmModule);
    }

    ionshared::Ptr<LlvmLoweringPass> llvmLoweringPass(const ionshared::Ptr<ionshared::LlvmModule> &module) {
        ionshared::Ptr<ionshared::SymbolTable<llvm::Module *>> modules = std::make_shared<ionshared::SymbolTable<llvm::Module *>>(ionshared::SymbolTable<llvm::Module *>({
            {module->getId(), module->unwrap()}
        }));

        // TODO: PassContext isn't associated with the calling scope/function in any way.
        ionshared::Ptr<LlvmLoweringPass> llvmLoweringPass = std::make_shared<LlvmLoweringPass>(
            std::make_shared<ionshared::PassContext>(),
            modules
        );

        if (!llvmLoweringPass->setModuleBuffer(module->getId())) {
            throw std::runtime_error("Could not set active module buffer during bootstrap process");
        }

        return llvmLoweringPass;
    }

    ionshared::Ptr<Function> emptyFunction(std::vector<ionshared::Ptr<Inst>> insts) {
        ionshared::Ptr<VoidType> returnType = TypeFactory::typeVoid();

        // TODO: Consider support for module here.
        ionshared::Ptr<Prototype> prototype = std::make_shared<Prototype>(
            test::constant::foobar,
            std::make_shared<Args>(),
            returnType,
            nullptr
        );

        ionshared::Ptr<BasicBlock> entrySection = std::make_shared<BasicBlock>(BasicBlockOpts{
            nullptr,
            BasicBlockKind::Entry,
            Const::basicBlockEntryId,
            std::move(insts)
        });

        // TODO: Fix mumbo-jumbo debugging code. -------------

        typedef PtrSymbolTable<BasicBlock> t;
        typedef ionshared::SymbolTable<ionshared::Ptr<BasicBlock>> tt;

        auto t1 = std::map<std::string, ionshared::Ptr<BasicBlock>>{
            {entrySection->name, entrySection}
        };

        t sections = std::make_shared<tt>(t1);

        // --------------------

        ionshared::Ptr<Function> function = std::make_shared<Function>(prototype, nullptr);
        ionshared::Ptr<FunctionBody> body = std::make_shared<FunctionBody>(function, sections);

        entrySection->parent = body;
        function->body = body;

        return function;
    }
}
