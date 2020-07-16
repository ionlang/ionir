#include <llvm/ADT/APInt.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constant.h>
#include <ionir/construct/value.h>
#include <ionir/passes/codegen/llvm_codegen_pass.h>

namespace ionir {
    void LlvmCodegenPass::visitIntegerValue(ionshared::Ptr<IntegerValue> node) {
        /**
         * Create the APInt (Arbitrary-precision integer)
         * to provide. Acts sort of an LLVM integer value
         * wrapper.
         */
        llvm::APInt apInt = llvm::APInt(
            (unsigned)node->getType()->getIntegerKind(),
            node->getValue(),
            node->getType()->getIsSigned()
        );

        ionshared::Ptr<Type> nodeType = node->getType();

        if (nodeType->getTypeKind() != TypeKind::Integer) {
            throw std::runtime_error("Integer value's type must be integer type");
        }

        this->visitIntegerType(nodeType->dynamicCast<IntegerType>());

        llvm::Type *type = this->typeStack.pop();

        // Finally, create the LLVM value constant.
        llvm::Constant *value = llvm::ConstantInt::get(type, apInt);

        this->addToScope(node, value);

        // Push the value onto the value stack.
        this->valueStack.push(value);
    }

    void LlvmCodegenPass::visitCharValue(ionshared::Ptr<CharValue> node) {
        this->requireContext();
        this->requireBuilder();

        llvm::Type *charType = llvm::Type::getInt8Ty(**this->contextBuffer);
        llvm::Constant *value = llvm::ConstantInt::get(charType, node->getValue());

        this->addToScope(node, value);
        this->valueStack.push(value);
    }

    void LlvmCodegenPass::visitStringValue(ionshared::Ptr<StringValue> node) {
        this->requireBuilder();

        // Create the global string pointer.
        llvm::Constant *value = this->builderBuffer->CreateGlobalStringPtr(node->getValue());

        this->addToScope(node, value);

        // Push the value onto the value stack.
        this->valueStack.push(value);
    }

    void LlvmCodegenPass::visitBooleanValue(ionshared::Ptr<BooleanValue> node) {
        this->requireContext();

        // Create the boolean type along with the LLVM value.
        llvm::IntegerType *type = llvm::Type::getInt1Ty(**this->contextBuffer);
        llvm::Constant *value = llvm::ConstantInt::get(type, llvm::APInt(1, node->getValue(), false));

        this->addToScope(node, value);

        // Push the resulting boolean constant onto the stack.
        this->valueStack.push(value);
    }
}
