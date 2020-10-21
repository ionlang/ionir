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
        // TODO: PassContext isn't associated with the calling scope/function in any way.
        std::shared_ptr<LlvmLoweringPass> llvmLoweringPass = std::make_shared<LlvmLoweringPass>(
            std::make_shared<ionshared::PassContext>()
        );

//        if (!llvmLoweringPass->setModuleBuffer(module->getId())) {
//            throw std::runtime_error("Could not set active module buffer during bootstrap process");
//        }

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

        std::shared_ptr<BasicBlock> entrySection = std::make_shared<BasicBlock>(
            nullptr,
            BasicBlockKind::Internal,
            std::move(instructions)
        );

        std::shared_ptr<Function> function =
            std::make_shared<Function>(prototype, nullptr);

        std::shared_ptr<FunctionBody> body =
            std::make_shared<FunctionBody>(
                function,
                std::set<std::shared_ptr<BasicBlock>>{entrySection}
            );

        entrySection->parent = body;
        function->body = body;

        return function;
    }
}
