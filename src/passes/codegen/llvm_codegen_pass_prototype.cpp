#include <llvm/ADT/APInt.h>
#include <llvm/IR/DerivedTypes.h>
#include <ionir/construct/value.h>
#include <ionir/passes/codegen/llvm_codegen_pass.h>

namespace ionir {
    void LlvmCodegenPass::visit(Ptr<Construct> node) {
        /**
         * Only instruct the node to visit this
         * instance and not its children, since
         * they're already visited by the other
         * member methods.
         */
        node->accept(*this);
    }

    void LlvmCodegenPass::visitExtern(Ptr<Extern> node) {
        if (node->getPrototype() == nullptr) {
            throw std::runtime_error("Unexpected external definition's prototype to be null");
        }
        else if (this->module->getFunction(node->getPrototype()->getId()) != nullptr) {
            throw std::runtime_error("Re-definition of extern not allowed");
        }

        // Visit the prototype.
        this->visitPrototype(node->getPrototype());

        // No need to push the resulting function onto the stack.
    }

    void LlvmCodegenPass::visitPrototype(Ptr<Prototype> node) {
        // Retrieve argument count from the argument vector.
        uint32_t argumentCount = node->getArgs()->getItems().getSize();

        // Create the argument buffer vector.
        std::vector<llvm::Type *> arguments = {};

        // Attempt to retrieve an existing function.
        llvm::Function *function = this->module->getFunction(node->getId());

        // A function with a matching identifier already exists.
        if (function != nullptr) {
            // Function already has a body, disallow re-definition.
            if (function->getBasicBlockList().empty()) {
                throw std::runtime_error("Cannot re-define function");
            }
                // If the function takes a different number of arguments, reject.
            else if (function->arg_size() != argumentCount) {
                throw std::runtime_error("Re-definition of function with a different amount arguments");
            }
        }
            // Otherwise, function will be created.
        else {
            for (uint32_t i = 0; i < argumentCount; ++i) {
                // TODO: Wrong type.
                arguments.push_back(llvm::Type::getDoubleTy(*this->context));
            }

            // Visit and pop the return type.
            this->visitType(node->getReturnType());

            llvm::Type *returnType = this->typeStack.pop();

            // TODO: Support for infinite arguments and hard-coded return type.
            // Create the function type.
            llvm::FunctionType *type =
                llvm::FunctionType::get(returnType, arguments, node->getArgs()->getIsInfinite());

            // Cast the value to a function, since we know getCallee() will return a function.
            function =
                llvm::dyn_cast<llvm::Function>(this->module->getOrInsertFunction(node->getId(), type).getCallee());

            // Set the function's linkage.
            function->setLinkage(llvm::GlobalValue::LinkageTypes::ExternalLinkage);
        }

        // Begin processing arguments. Argument count must be the same.
        if (argumentCount != function->arg_size()) {
            throw std::runtime_error("Expected argument count to be the same as the function's argument count");
        }

        int i = 0;

        for (auto &arg : function->args()) {
            // TODO: getItems() no longer a vector; cannot index by index, only key.
            // Retrieve the name element from the argument tuple.
            //            std::string name = node->getArgs()->getItems()[i].second;

            // Name the argument.
            //            arg.setName(name);

            // Increment the counter to prepare for next iteration.
            i++;
        }

        this->valueStack.push(function);
    }

    void LlvmCodegenPass::visitFunction(Ptr<Function> node) {
        if (!node->verify()) {
            throw std::runtime_error("Function verification failed");
        }
        else if (this->module->getFunction(node->getPrototype()->getId()) != nullptr) {
            throw std::runtime_error("A function with the same identifier has been already previously defined");
        }

        // Clear named values.
        this->namedValues.clear();

        // Visit the prototype.
        this->visitPrototype(node->getPrototype());

        // Retrieve the resulting function off the stack.
        auto *function = this->valueStack.popAs<llvm::Function>();

        // Set the function buffer.
        this->function = function;

        // Visit the function's body.
        this->visitBlock(node->getBody());

        // TODO: Verify the resulting LLVM function (through LLVM).

        // Push the function back onto the stack.
        this->valueStack.push(function);
    }
}
