#pragma once

#include <iostream>
#include <array>
#include <string>
#include <ionir/passes/codegen/llvm_lowering_pass.h>
#include "util.h"
#include "../../../test/pch.h"

namespace ionir::test::comparison {
    /**
     * Compare 2 strings. Both strings are trimmed before
     * comparison.
     */
    [[nodiscard]] bool strings(std::string stringA, std::string stringB);

    /**
     * Compare an LLVM IR output string with a stored LLVM IR
     * file. Both inputs are trimmed before comparison. Returns
     * false if the stored LLVM IR file does not exist.
     */
    [[nodiscard]] bool ir(std::string output, const std::string &fileName);

    [[nodiscard]] bool ir(
        const ionshared::Ptr<LlvmCodegenPass> &llvmCodegenPass,
        const std::string &fileName
    );
}
