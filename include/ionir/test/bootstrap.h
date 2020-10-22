#pragma once

#include <vector>
#include <ionshared/llvm/llvm_module.h>
#include <ionir/construct/instruction.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>
#include "util.h"

namespace ionir::test::bootstrap {
    [[nodiscard]] std::shared_ptr<LlvmLoweringPass> llvmLoweringPass(
        std::shared_ptr<llvm::Module> module =
            std::make_shared<llvm::Module>("test_module", *std::make_shared<llvm::LLVMContext>())
    );

    [[nodiscard]] std::shared_ptr<Function> emptyFunction(
        std::vector<std::shared_ptr<Instruction>> instructions = {}
    );
}
