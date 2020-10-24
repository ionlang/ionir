#include <ionir/const/const.h>
#include <ionir/test/const.h>
#include <ionir/test/bootstrap.h>

namespace ionir::test::bootstrap {
    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass(
        std::shared_ptr<llvm::Module> module
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
        // TODO: Use 'module' parameter.

        std::shared_ptr<VoidType> returnType = TypeFactory::typeVoid();

        // TODO: Consider support for module here.
        std::shared_ptr<Prototype> prototype = Prototype::make(
            test::constant::foobar,
            std::make_shared<Args>(),
            returnType
        );

        returnType->parent = prototype;

        std::shared_ptr<BasicBlock> entrySection = BasicBlock::make(
            instructions
        );

        std::shared_ptr<Function> function = Function::make(
            prototype,
            std::vector<std::shared_ptr<BasicBlock>>{entrySection}
        );

        return function;
    }
}
