#include <llvm/ADT/APInt.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constant.h>
#include <ionir/construct/value.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    void LlvmLoweringPass::visitIntegerLiteral(std::shared_ptr<IntegerLiteral> construct) {
        /**
         * Create the APInt (Arbitrary-precision integer) to provide.
         * Acts sort of an LLVM integer value wrapper.
         */
        llvm::APInt llvmApInt{
            (unsigned)construct->type->integerKind,
            construct->value,
            construct->type->isSigned
        };

        if (construct->type->typeKind != TypeKind::Integer) {
            throw std::runtime_error("Integer value's type must be integer type");
        }

        this->valueSymbolTable.set(construct, llvm::ConstantInt::get(
            this->eagerVisitType(construct->type),
            llvmApInt
        ));

//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitCharLiteral(std::shared_ptr<CharLiteral> construct) {
        this->valueSymbolTable.set(
            construct,

            llvm::ConstantInt::get(
                llvm::Type::getInt8Ty(
                    this->llvmBuffers.modules.forceGetTopItem()
                        ->getContext()
                ),

                construct->value
            )
        );
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitStringLiteral(std::shared_ptr<StringLiteral> construct) {
        this->valueSymbolTable.set(
            construct,

            this->llvmBuffers.makeBuilder()
                .CreateGlobalStringPtr(construct->value)
        );
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitBooleanLiteral(std::shared_ptr<BooleanLiteral> construct) {
        // Create the boolean type along with the LLVM value.
        this->valueSymbolTable.set(construct, llvm::ConstantInt::get(
            llvm::Type::getInt1Ty(
                this->llvmBuffers.modules.forceGetTopItem()
                    ->getContext()
            ),

            llvm::APInt(1, construct->value, false)
        ));
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitOperationValue(std::shared_ptr<OperationValue> construct) {
        std::optional<llvm::Value*> llvmRightSideValue = std::nullopt;
        bool isInteger = construct->leftSideValue->type->typeKind == TypeKind::Integer;

        /**
         * NOTE: During lowering it is assumed that both types are
         * compatible (equal), since there is no implicit conversions
         * done by the compiler. This assurance comes from the type-checking
         * pass, which must occur prior to lowering.
         */
        if (!isInteger && construct->leftSideValue->type->typeKind != TypeKind::Decimal) {
            // TODO: Internal error?
            throw std::runtime_error("Value type of operation must be numerical");
        }

        auto requireAndVisitRightSide = [&]{
            if (!ionshared::util::hasValue(construct->rightSideValue)) {
                throw std::runtime_error("Right side must have a value for this operation");
            }

            llvmRightSideValue = this->eagerVisitValue(*construct->rightSideValue);
        };

        llvm::Value* llvmLeftSideValue = this->eagerVisitValue(construct->leftSideValue);
        llvm::Instruction::BinaryOps llvmBinaryOperator;
        llvm::Instruction::UnaryOps llvmUnaryOperator;

        switch (construct->operatorKind) {
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
                if (ionshared::util::hasValue(construct->rightSideValue)) {
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

        llvm::IRBuilder<> llvmBuilder = this->llvmBuffers.makeBuilder();

        this->valueSymbolTable.set(construct, ionshared::util::hasValue(construct->rightSideValue)
            ? llvmBuilder.CreateBinOp(
                llvmBinaryOperator,
                llvmLeftSideValue,
                *llvmRightSideValue
            )

            : llvmBuilder.CreateUnOp(
                llvmUnaryOperator,
                llvmLeftSideValue
            )
        );
    }
}
