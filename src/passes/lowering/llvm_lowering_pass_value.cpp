#include <llvm/ADT/APInt.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constant.h>
#include <ionir/construct/value.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    void LlvmLoweringPass::visitIntegerLiteral(ionshared::Ptr<IntegerLiteral> node) {
        /**
         * Create the APInt (Arbitrary-precision integer) to provide.
         * Acts sort of an LLVM integer value wrapper.
         */
        llvm::APInt apInt(
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
            this->makeLlvmBuilder()->CreateGlobalStringPtr(node->value)
        );
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitBooleanLiteral(ionshared::Ptr<BooleanLiteral> node) {
        this->requireContext();

        // Create the boolean type along with the LLVM value.
        llvm::IntegerType *type = llvm::Type::getInt1Ty(**this->buffers.llvmContext);

        llvm::Constant *value = llvm::ConstantInt::get(
            type,
            llvm::APInt(1, node->value, false)
        );

        this->valueStack.push(value);
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitOperationValue(ionshared::Ptr<OperationValue> node) {
        llvm::IRBuilder<> llvmBuilder = this->requireBuilder();
        std::optional<llvm::Value*> llvmRightSideValue = std::nullopt;
        bool isInteger = node->leftSideValue->type->typeKind == TypeKind::Integer;

        /**
         * NOTE: During lowering it is assumed that both types are
         * compatible (equal), since there is no implicit conversions
         * done by the compiler. This assurance comes from the type-checking
         * pass, which must occur prior to lowering.
         */
        if (!isInteger && node->leftSideValue->type->typeKind != TypeKind::Decimal) {
            // TODO: Internal error?
            throw std::runtime_error("Value type of operation must be numerical");
        }

        auto requireAndVisitRightSide = [&]{
            if (!ionshared::util::hasValue(node->rightSideValue)) {
                throw std::runtime_error("Right side must have a value for this operation");
            }

            this->visit(*node->rightSideValue);
            llvmRightSideValue = this->valueStack.pop();
        };

        this->visit(node->leftSideValue);

        llvm::Value* llvmLeftSideValue = this->valueStack.pop();
        llvm::Value* result;
        llvm::Instruction::BinaryOps llvmBinaryOperator;
        llvm::Instruction::UnaryOps llvmUnaryOperator;

        switch (node->operatorKind) {
            case OperatorKind::Addition: {
                requireAndVisitRightSide();

                llvmBinaryOperator = isInteger
                    ? llvm::Instruction::BinaryOps::Add
                    : llvm::Instruction::BinaryOps::FAdd;

                break;
            }

            case OperatorKind::Multiplication: {
                requireAndVisitRightSide();

                llvmBinaryOperator = isInteger
                    ? llvm::Instruction::BinaryOps::Mul
                    : llvm::Instruction::BinaryOps::FMul;

                break;
            }

            case OperatorKind::Subtraction: {
                if (ionshared::util::hasValue(node->rightSideValue)) {
                    llvmBinaryOperator = isInteger
                        ? llvm::Instruction::BinaryOps::Sub
                        : llvm::Instruction::BinaryOps::FSub;

                    break;
                }

                // TODO: Implement lowering of integer neg. operator (via 0 - value?).
                llvmUnaryOperator = isInteger
                    ? throw std::runtime_error("Not yet implemented")
                    : llvm::Instruction::UnaryOps::FNeg;

                break;
            }

            case OperatorKind::And: {
                llvmBinaryOperator = llvm::Instruction::BinaryOps::And;

                break;
            }

            case OperatorKind::Or: {
                llvmBinaryOperator = llvm::Instruction::BinaryOps::Or;

                break;
            }

                // TODO: Add support for missing operators.

            default: {
                throw std::runtime_error("Unsupported operator kind");
            }
        }

        if (ionshared::util::hasValue(node->rightSideValue)) {
            result = llvmBuilder.CreateBinOp(
                llvmBinaryOperator,
                llvmLeftSideValue,
                *llvmRightSideValue
            );
        }
        else {
            result = llvmBuilder.CreateUnOp(
                llvmUnaryOperator,
                llvmLeftSideValue
            );
        }

        this->valueStack.push(result);
    }
}
