#define IONIR_LLVM_IR_FILE_EXT ".ll"
#define IONIR_TESTS_OUTPUT_FOLDER_PATH "output"

#include <ionshared/llvm/llvm_module.h>
#include <ionir/test/comparison.h>
#include <ionir/test/filesystem.h>

namespace ionir::test::comparison {
    bool strings(std::string stringA, std::string stringB) {
        return util::trim(std::move(stringA)) == util::trim(std::move(stringB));
    }

    bool ir(std::string output, const std::string& fileName) {
        std::optional<std::string> contents = fs::readTestFile(
            fs::joinPaths(
                IONIR_TESTS_OUTPUT_FOLDER_PATH,
                fileName + IONIR_LLVM_IR_FILE_EXT
            )
        );

        // TODO: Consider returning int or enum for better verbosity.
        // File contents could not be retrieved. Fail process.
        if (!contents.has_value()) {
            return false;
        }

        // Trim and compare expected output and actual file content.
        return util::trim(std::move(output)) == util::trim(*contents);
    }

    bool ir(
        const std::shared_ptr<LlvmLoweringPass>& llvmLoweringPass,
        const std::string& fileName
    ) {
        // TODO: Verify that a module was emitted / contained there.
        std::optional<llvm::Module*> llvmModuleBuffer =
            llvmLoweringPass->getModules()->unwrap().begin()->second;

        if (!ionshared::util::hasValue(llvmModuleBuffer)) {
            throw std::runtime_error("Module buffer in LlvmCodegenPass is not set");
        }

        return comparison::ir(ionshared::LlvmModule(*llvmModuleBuffer).makeIr(), fileName);
    }
}
