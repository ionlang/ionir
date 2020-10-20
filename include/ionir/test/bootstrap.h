#pragma once

#include <vector>
#include <ionshared/llvm/llvm_module.h>
#include <ionir/construct/inst.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>
#include "util.h"

namespace ionir::test::bootstrap {
    [[nodiscard]] std::shared_ptr<ionshared::LlvmModule> llvmModule(
        const std::string& identifier = "test"
    );

    [[nodiscard]] std::shared_ptr<LlvmLoweringPass> llvmLoweringPass(
        const std::shared_ptr<ionshared::LlvmModule>& module = llvmModule()
    );

    [[nodiscard]] std::shared_ptr<Function> emptyFunction(
        std::vector<std::shared_ptr<Instruction>> instructions = {}
    );
}
