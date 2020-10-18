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

        this->valueSymbolTable.set(construct, std::shared_ptr<llvm::Value>(
            llvm::ConstantInt::get(
                this->typeSafeEarlyVisitOrLookup(construct->type).get(),
                llvmApInt
            )
        ));

//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitCharLiteral(std::shared_ptr<CharLiteral> construct) {
        this->requireContext();
        this->requireBuilder();

        llvm::Type *charType = llvm::Type::getInt8Ty(**this->buffers.llvmContext);

        this->valueSymbolTable.set(
            construct,

            std::shared_ptr<llvm::Value>(
                llvm::ConstantInt::get(charType, construct->value)
            )
        );
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitStringLiteral(std::shared_ptr<StringLiteral> construct) {
        this->requireBuilder();

        this->valueSymbolTable.set(
            construct,

            std::shared_ptr<llvm::Value>(
                this->makeLlvmBuilder()->CreateGlobalStringPtr(construct->value)
            )
        );
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitBooleanLiteral(std::shared_ptr<BooleanLiteral> node) {
        this->requireContext();

        // Create the boolean type along with the LLVM value.
        llvm::IntegerType *type = llvm::Type::getInt1Ty(**this->buffers.llvmContext);

        this->valueSymbolTable.set(node, std::shared_ptr<llvm::Value>(
            llvm::ConstantInt::get(
                type,
                llvm::APInt(1, node->value, false)
            )
        ));
//        this->addToScope(node, value);
    }

    void LlvmLoweringPass::visitOperationValue(std::shared_ptr<OperationValue> node) {
        llvm::IRBuilder<> llvmBuilder = this->requireBuilder();
        std::optional<std::shared_ptr<llvm::Value>> llvmRightSideValue = std::nullopt;
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

            llvmRightSideValue = this->valueSafeEarlyVisitOrLookup(*node->rightSideValue);
        };

        std::shared_ptr<llvm::Value> llvmLeftSideValue =
            this->valueSafeEarlyVisitOrLookup(node->leftSideValue);

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

        this->valueSymbolTable.set(node, std::shared_ptr<llvm::Value>(
            ionshared::util::hasValue(node->rightSideValue)
                ? llvmBuilder.CreateBinOp(
                    llvmBinaryOperator,
                    llvmLeftSideValue.get(),
                    llvmRightSideValue->get()
                )

                : llvmBuilder.CreateUnOp(
                    llvmUnaryOperator,
                    llvmLeftSideValue.get()
                )
        ));
    }
}
