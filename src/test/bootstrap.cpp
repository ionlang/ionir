#include <ionir/const/const.h>
#include <ionir/test/const.h>
#include <ionir/test/bootstrap.h>

namespace ionir::test::bootstrap {
    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass(
        const std::shared_ptr<Module>& module
    ) {
        // TODO: PassContext isn't associated with the calling scope/function in any way.
        std::shared_ptr<LlvmLoweringPass> llvmLoweringPass =
            std::make_shared<LlvmLoweringPass>(
                std::make_shared<ionshared::PassContext>()
            );

        llvmLoweringPass->visitModule(module);

        llvmLoweringPass->llvmBuffers.modules.push(
            llvmLoweringPass->llvmModules->unwrap().begin()->second
        );

        llvmLoweringPass->localBuffers.modules.push(module);

        return llvmLoweringPass;
    }

    std::shared_ptr<Function> emptyFunction(
        const std::vector<std::shared_ptr<Instruction>>& instructions
    ) {
        // TODO: Consider support for module here.

        std::shared_ptr<Function> function = std::make_shared<Function>(
            std::make_shared<Prototype>(
                test::constant::foo,
                std::make_shared<Args>(),
                std::make_shared<TypeVoid>()
            ),

            std::vector<std::shared_ptr<BasicBlock>>{
                std::make_shared<BasicBlock>(
                    instructions
                )
            }
        );

        return function;
    }
}
