#include <llvm/ADT/APInt.h>
#include <llvm/IR/DerivedTypes.h>
#include <ionir/construct/value.h>
#include <ionir/diagnostics/diagnostic.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    void LlvmLoweringPass::visit(ionshared::Ptr<Construct> node) {
        /**
         * Only instruct the node to visit this instance and not
         * its children, since they're already manually visited by
         * the other member methods.
         */
        node->accept(*this);
    }

    void LlvmLoweringPass::visitExtern(ionshared::Ptr<Extern> node) {
        if (this->symbolTable.contains(node)) {
            return;
        }

        this->requireModule();

        IONIR_PASS_INTERNAL_ASSERT(node->prototype != nullptr)

        llvm::Function *existingExternDefinition =
            (*this->buffers.llvmModule)->getFunction(node->prototype->name);

        if (existingExternDefinition != nullptr) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::externRedefinition)
                ->finish();

            throw std::runtime_error("Awaiting new diagnostic buffer checking");
        }

        // Visit the prototype.
        this->visitPrototype(node->prototype);

        llvm::Function *llvmExtern = this->valueStack.popAs<llvm::Function>();

        this->symbolTable.set(node, llvmExtern);
        this->valueStack.push(llvmExtern);
    }

    void LlvmLoweringPass::visitPrototype(ionshared::Ptr<Prototype> node) {
        this->requireModule();
        this->requireContext();

        auto argsMap = node->args->getItems();
        auto &argsNativeMap = argsMap->unwrapConst();

        // Retrieve argument count from the argument vector.
        uint32_t argumentCount = argsMap->getSize();

        // Create the argument buffer vector.
        std::vector<llvm::Type *> llvmArgumentTypes = {};

        // Attempt to retrieve an existing function.
        llvm::Function *llvmFunction = (*this->buffers.llvmModule)->getFunction(node->name);

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
            for (const auto &[id, arg] : argsNativeMap) {
                arg.first->accept(*this);
                llvmArgumentTypes.push_back(this->typeStack.pop());
            }

            // Visit and pop the return type.
            node->returnType->accept(*this);

            llvm::Type *llvmReturnType = this->typeStack.pop();

            // TODO: Support for infinite arguments and hard-coded return type.
            // Create the function type.
            llvm::FunctionType *llvmFunctionType = llvm::FunctionType::get(
                llvmReturnType,
                llvmArgumentTypes,
                node->args->getIsVariable()
            );

            // Cast the value to a function, since we know getCallee() will return a function.
            llvmFunction = llvm::dyn_cast<llvm::Function>(
                (*this->buffers.llvmModule)->getOrInsertFunction(node->name, llvmFunctionType).getCallee()
            );

            // Set the function's linkage.
            llvmFunction->setLinkage(llvm::GlobalValue::LinkageTypes::ExternalLinkage);
        }

        // Begin processing arguments. Argument count must be the same.
        if (argumentCount != llvmFunction->arg_size()) {
            throw std::runtime_error("Expected argument count to be the same as the function's argument count");
        }

        uint32_t argCounter = 0;
        uint32_t llvmArgCounter = 0;

        // TODO: Simplify method of naming LLVM arguments, as this implementation is inefficient.
        for (const auto &[id, arg] : argsNativeMap) {
            for (auto &llvmArgument : llvmFunction->args()) {
                if (llvmArgCounter == argCounter) {
                    llvmArgument.setName(id);
                    llvmArgCounter = 0;

                    break;
                }

                llvmArgCounter++;
            }

            argCounter++;
        }

        this->valueStack.push(llvmFunction);
    }

    void LlvmLoweringPass::visitFunction(ionshared::Ptr<Function> node) {
        if (this->symbolTable.contains(node)) {
            return;
        }

        this->requireModule();

        if (!node->verify()) {
            throw std::runtime_error("Function verification failed");
        }
        else if ((*this->buffers.llvmModule)->getFunction(node->prototype->name) != nullptr) {
            throw std::runtime_error("A function with the same identifier has been already previously defined");
        }

        // Clear named values.
        this->namedValues.clear();

        // Visit the prototype.
        this->visitPrototype(node->prototype);

        // Retrieve the resulting function off the stack.
        llvm::Function *llvmFunction = this->valueStack.popAs<llvm::Function>();

        // Set the function buffer.
        this->buffers.llvmFunction = llvmFunction;

        // Visiting the function body's yields no value to the value stack.
        this->visitFunctionBody(node->body);

        // TODO: Verify the resulting LLVM function (through LLVM)?

        this->symbolTable.set(node, llvmFunction);
        this->valueStack.push(llvmFunction);
    }
}
