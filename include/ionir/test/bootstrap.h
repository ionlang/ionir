#pragma once

#include <vector>
#include <ionshared/llvm/llvm_module.h>
#include <ionir/construct/instruction.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>
#include "util.h"
#include "const.h"

namespace ionir::test::bootstrap {
    [[nodiscard]] std::shared_ptr<LlvmLoweringPass> llvmLoweringPass(
        const std::shared_ptr<Module>& module =
            std::make_shared<Module>(std::make_shared<Identifier>(test::constant::foo))
    );

    /**
     * Create an empty function without a parent module. Do
     * note that a construct without a parent is technically
     * ill-formed.
     */
    [[nodiscard]] std::shared_ptr<Function> emptyFunction(
        const std::vector<std::shared_ptr<Instruction>>& instructions = {}
    );
}
