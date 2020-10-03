#pragma once

#include "assembly_builder.h"
#include "x86_pointer.h"

namespace ionir {
    struct x86AssemblyBuilder : AssemblyBuilder<x86AssemblyBuilder> {
        ionshared::Ptr<x86AssemblyBuilder> makeJmp(std::string target) noexcept;

        ionshared::Ptr<x86AssemblyBuilder> makeJmp(const x86Pointer &pointer) noexcept;
    };
}
