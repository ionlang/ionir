#include <iostream>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/DerivedTypes.h>
#include <ionir/construct/value.h>
#include <ionir/diagnostics/diagnostic.h>
#include <ionir/passes/semantic/construct_verification_pass.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    void LlvmLoweringPass::visit(std::shared_ptr<Construct> node) {
        // Prevent construct from being emitted more than once.
        if (this->valueSymbolTable.contains(node)) {
            return;
        }

        /**
         * Only instruct the construct to visit this instance and not
         * its children, since they're already manually visited by
         * the other member methods.
         */
        node->accept(*this);
    }

    bool LlvmLoweringPass::initialize(ionshared::PassInfo& info) {
        /**
         * In order to prevent segmentation faults, the construct
         * tree must be validated before running this pass.
         */
        info.addRequirement<ConstructVerificationPass>();

        return true;
    }

    void LlvmLoweringPass::finish() {
        bool isBuffersEmpty = this->llvmBuffers.modules.isEmpty()
            && this->llvmBuffers.functions.isEmpty()
            && this->llvmBuffers.basicBlocks.isEmpty();

        // At this point, all LLVM buffers should be empty.
        if (!isBuffersEmpty) {
            // TODO: Use diagnostics API.
            std::cout << "Internal warning: LLVM buffers were not empty upon finish" << std::endl;
        }
    }

    void LlvmLoweringPass::visitExtern(std::shared_ptr<Extern> construct) {
        this->requireModule();

        llvm::Function* existingExternDefinition =
            (*this->buffers.module)->getFunction(construct->prototype->name);

        if (existingExternDefinition != nullptr) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::externRedefinition)
                ->finish();

            throw std::runtime_error("Awaiting new diagnostic buffer checking");
        }

        this->valueSymbolTable.set(
            construct,

            this->valueSafeEarlyVisitOrLookup<llvm::Function>(
                construct->prototype
            )
        );
    }

    void LlvmLoweringPass::visitPrototype(std::shared_ptr<Prototype> construct) {
        this->requireModule();
        this->requireContext();

        auto argsMap = construct->args->items;
        auto& argsNativeMap = argsMap->unwrapConst();

        // Retrieve argument count from the argument vector.
        uint32_t argumentCount = argsMap->getSize();

        // Create the argument buffer vector.
        std::vector<llvm::Type*> llvmArgumentTypes = {};

        // Attempt to retrieve an existing function.
        llvm::Function* llvmFunction = (*this->buffers.module)->getFunction(construct->name);

        // A function with a matching identifier already exists.
        if (llvmFunction != nullptr) {
            // Function already has a body, disallow re-definition.
            if (llvmFunction->getBasicBlockList().empty()) {
                throw std::runtime_error("Cannot re-define function");
            }
            // If the function takes a different number of arguments, reject.
            else if (llvmFunction->arg_size() != argumentCount) {
                this->context->diagnosticBuilder
                    ->bootstrap(diagnostic::functionRedefinitionDiffArgs)
                    ->finish();

                throw std::runtime_error("Awaiting new diagnostic buffer checking");
            }
        }
        // Otherwise, function will be created.
        else {
            for (const auto& [id, arg] : argsNativeMap) {
                llvmArgumentTypes.push_back(
                    this->typeSafeEarlyVisitOrLookup(arg.first).get()
                );
            }

            llvm::FunctionType* llvmFunctionType = llvm::FunctionType::get(
                this->typeSafeEarlyVisitOrLookup(construct->returnType).get(),
                llvmArgumentTypes,
                construct->args->isVariable
            );

            // Cast the value to a function, since we know getCallee() will return a function.
            llvmFunction = llvm::dyn_cast<llvm::Function>(
                (*this->buffers.module)->getOrInsertFunction(construct->name, llvmFunctionType).getCallee()
            );

            // Set the function's linkage.
            llvmFunction->setLinkage(llvm::GlobalValue::LinkageTypes::ExternalLinkage);
        }

        // Begin processing arguments. Argument count must be the same.
        if (argumentCount != llvmFunction->arg_size()) {
            throw std::runtime_error("Expected argument count to be the same as the function's argument count");
        }

        size_t argCounter = 0;
        size_t llvmArgCounter = 0;

        // TODO: Simplify method of naming LLVM arguments, as this implementation is inefficient.
        for (const auto& [id, arg] : argsNativeMap) {
            for (auto& llvmArgument : llvmFunction->args()) {
                if (llvmArgCounter == argCounter) {
                    llvmArgument.setName(id);
                    llvmArgCounter = 0;

                    break;
                }

                llvmArgCounter++;
            }

            argCounter++;
        }

        this->valueSymbolTable.set(
            construct,
            std::shared_ptr<llvm::Value>(llvmFunction)
        );
    }

    void LlvmLoweringPass::visitFunction(std::shared_ptr<Function> construct) {
        std::shared_ptr<llvm::Module> llvmModuleBuffer =
            this->llvmBuffers.modules.forceGetTopItem();

        if (llvmModuleBuffer->getFunction(construct->prototype->name) != nullptr) {
            throw std::runtime_error("A function with the same identifier has been already previously defined");
        }

        std::shared_ptr<llvm::Function> llvmFunction =
            this->valueSafeEarlyVisitOrLookup<llvm::Function>(construct->prototype);

        this->llvmBuffers.functions.push(llvmFunction);

        // Visiting the function body's yields no value to the value stack.
        this->visit(construct->body);

        // TODO: Verify the resulting LLVM function (through LLVM)?

        this->llvmBuffers.functions.forcePop();
        this->valueSymbolTable.set(construct, llvmFunction);
    }
}
