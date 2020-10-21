#pragma once

#include <string>
#include <ionshared/llvm/llvm_module.h>

namespace ionir::driver {
    ionshared::LlvmIrString run(const std::string& input);
}
