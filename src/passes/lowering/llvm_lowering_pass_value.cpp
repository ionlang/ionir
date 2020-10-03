#include <llvm/ADT/APInt.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constant.h>
#include <ionir/construct/value.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    void LlvmLoweringPass::visitIntegerLiteral(ionshared::Ptr<IntegerLiteral> node) {
        /**
         * Create the APInt (Arbitrary-precision integer)
         * to provide. Acts sort of an LLVM integer value
         * wrapper.
         */
        llvm::APInt apInt = llvm::APInt(
            (unsigned)node->type->integerKind,
            node->value,
            node->type->isSigned
        );

        ionshared::Ptr<Type> nodeType = node->type;

        if (nodeType->typeKind != TypeKind::Integer) {
            throw std::runtime_error("Integer value's type must be integer type");
        }

        this->visitIntegerType(nodeType->dynamicCast<IntegerType>());

        llvm::Type *type = this->typeStack.pop();

        // Finally, create the LLVM value constant.
        llvm::Constant *value = llvm::ConstantInt::get(type, apInt);

        this->valueStack.push(value);
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitCharLiteral(ionshared::Ptr<CharLiteral> node) {
        this->requireContext();
        this->requireBuilder();

        llvm::Type *charType = llvm::Type::getInt8Ty(**this->buffers.llvmContext);
        llvm::Constant *value = llvm::ConstantInt::get(charType, node->value);

        this->valueStack.push(value);
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitStringLiteral(ionshared::Ptr<StringLiteral> node) {
        this->requireBuilder();

        this->valueStack.push(
            this->getLlvmBuilder()->CreateGlobalStringPtr(node->value)
        );
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitBooleanLiteral(ionshared::Ptr<BooleanLiteral> node) {
        this->requireContext();

        // Create the boolean type along with the LLVM value.
        llvm::IntegerType *type = llvm::Type::getInt1Ty(**this->buffers.llvmContext);
        llvm::Constant *value = llvm::ConstantInt::get(type, llvm::APInt(1, node->value, false));

        this->valueStack.push(value);
//        this->addToScope(node, value);
    }
}
