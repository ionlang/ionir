#include <utility>
#include <ionir/const/const.h>
#include <ionir/test/const.h>
#include <ionir/test/bootstrap.h>

namespace ionir::test::bootstrap {
    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass(
        std::shared_ptr<Module> module
    ) {
        // TODO: PassContext isn't associated with the calling scope/function in any way.
        std::shared_ptr<LlvmLoweringPass> llvmLoweringPass =
            std::make_shared<LlvmLoweringPass>(
                std::make_shared<ionshared::PassContext>()
            );

        llvmLoweringPass->visitModule(std::move(module));

        llvmLoweringPass->llvmBuffers.modules.push(
            llvmLoweringPass->llvmModules->unwrap().begin()->second
        );

        return llvmLoweringPass;
    }

    std::shared_ptr<Function> emptyFunction(
        const std::vector<std::shared_ptr<Instruction>>& instructions
    ) {
        // TODO: Consider support for module here.

        std::shared_ptr<Function> function = Function::make(
            Prototype::make(
                test::constant::foo,
                std::make_shared<Args>(),
                std::make_shared<VoidType>()
            ),

            std::vector<std::shared_ptr<BasicBlock>>{BasicBlock::make(
                instructions
            )}
        );

        return function;
    }
}
