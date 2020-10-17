#pragma once

#include <vector>
#include <ionshared/llvm/llvm_module.h>
#include <ionir/construct/inst.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>
#include "util.h"

namespace ionir::test::bootstrap {
    [[nodiscard]] ionshared::Ptr<ionshared::LlvmModule> llvmModule(
        const std::string &identifier = "test"
    );

    [[nodiscard]] ionshared::Ptr<LlvmLoweringPass> llvmLoweringPass(
        const ionshared::Ptr<ionshared::LlvmModule>& module = llvmModule()
    );

    [[nodiscard]] ionshared::Ptr<Function> emptyFunction(
        std::vector<ionshared::Ptr<Inst>> insts = {}
    );
}
