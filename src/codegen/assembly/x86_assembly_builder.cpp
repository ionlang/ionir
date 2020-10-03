#include <utility>
#include <ionir/codegen/assembly/x86_assembly_builder.h>

namespace ionir {
    ionshared::Ptr<x86AssemblyBuilder> x86AssemblyBuilder::makeJmp(std::string target) noexcept {
        return this->makeInst("jmp", std::move(target));
    }

    ionshared::Ptr<x86AssemblyBuilder> x86AssemblyBuilder::makeJmp(const x86Pointer &pointer) noexcept {
        // TODO
        return this->makeJmp("foo");
    }
}
