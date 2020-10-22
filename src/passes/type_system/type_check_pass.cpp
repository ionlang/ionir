#include <ionir/type_system/type_util.h>
#include <ionir/passes/semantic/entry_point_check_pass.h>
#include <ionir/passes/type_system/type_check_pass.h>

namespace ionir {
    TypeCheckPass::TypeCheckPass(std::shared_ptr<ionshared::PassContext> context) :
        Pass(std::move(context)) {
        //
    }

    bool TypeCheckPass::initialize(ionshared::PassInfo& info) {
        // TODO: Technically, we don't NEED to check for an entry point during type-check.
//        info.addRequirement<EntryPointCheckPass>();

        return true;
    }

    void TypeCheckPass::visitFunction(std::shared_ptr<Function> node) {
        ionshared::OptPtr<BasicBlock> entryBasicBlock = std::nullopt;

        if (!node->body->basicBlocks.empty()) {
            entryBasicBlock = node->body->basicBlocks.front();
        }

        if (!ionshared::util::hasValue(entryBasicBlock)) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::functionMissingEntryBasicBlock)
                ->formatMessage(node->prototype->name)
                ->finish();

            return;
        }

        TypeKind parentFunctionPrototypeReturnTypeKind = node
            ->body
            ->getUnboxedParent()
            ->prototype
            ->returnType
            ->typeKind;

        /**
         * Entry basic blocks must contain at least a single terminal
         * instruction if the parent function does not return void. If
         * it returns void and there is no terminal instruction, the
         * LLVM codegen pass will implicitly append a return instruction
         * with no value.
         */
        if (parentFunctionPrototypeReturnTypeKind != TypeKind::Void) {
            std::vector<std::shared_ptr<Instruction>> instructions =
                entryBasicBlock->get()->instructions;

            // TODO: CRITICAL! There may be more than a single terminal statement on basic blocks? Technically speaking LLVM does not allow that to EXECUTE, however, it can OCCUR.
            ionshared::OptPtr<Instruction> terminalInst =
                entryBasicBlock->get()->findTerminalInst();

            if (instructions.empty() || !ionshared::util::hasValue(terminalInst)) {
                this->context->diagnosticBuilder
                    ->bootstrap(diagnostic::functionReturnValueMissing)
                    ->formatMessage(node->prototype->name)
                    ->finish();
            }
        }
    }

    void TypeCheckPass::visitReturnInst(std::shared_ptr<ReturnInst> node) {
        std::shared_ptr<Construct> possibleFunctionParent =
            node->getUnboxedParent()->getUnboxedParent()->getUnboxedParent();

        IONIR_PASS_INTERNAL_ASSERT(
            possibleFunctionParent->constructKind == ConstructKind::Function
        )

        std::shared_ptr<Function> function = possibleFunctionParent->dynamicCast<Function>();
        std::shared_ptr<Type> functionReturnType = function->prototype->returnType;
        ionshared::OptPtr<Construct> returnStatementValue = node->value;
        bool returnStatementValueSet = ionshared::util::hasValue(returnStatementValue);

        /**
         * Functions whose prototype's return type is non-void must provide
         * a value to the return instruction.
         */
        if ((functionReturnType->typeKind != TypeKind::Void) && !returnStatementValueSet) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::functionReturnValueMissing)
                ->formatMessage(function->prototype->name)
                ->finish();

            // TODO: Can it be made optional/continue?
            return;
        }
        else if (returnStatementValueSet) {
            switch (returnStatementValue->get()->constructKind) {
                /**
                 * Return instruction's value is a construct derived from Value<>.
                 * Compare its type to the function's return type.
                 */
                case ConstructKind::Value: {
                    /**
                     * At this point, the function returns a non-void value. Abstract its
                     * return value's type construct.
                     */
                    std::shared_ptr<Type> returnInstValueType =
                        returnStatementValue->get()->staticCast<Value<>>()->type;

                    /**
                     * Function's return type and the return instruction's value's type
                     * do not match. Issue a type error. Even if it's returning compatible
                     * types (such as different bit length integer values) no implicit conversions
                     * are made by the compiler (everything on the IR language must be explicit).
                     */
                    if (!returnInstValueType->isSameAs(functionReturnType)) {
                        this->context->diagnosticBuilder
                            ->bootstrap(diagnostic::functionReturnValueTypeMismatch)

                            // TODO: Format-in return types.
                            ->formatMessage(function->prototype->name, "pending", "pending")

                            ->finish();
                    }

                    break;
                }

                /**
                 * Return instruction's value is set, but it's a reference. The reference
                 * must be resolved then compared.
                 */
                case ConstructKind::Ref: {
                    // TODO
                    throw std::runtime_error("Not implemented");

                    break;
                }

                // TODO: What about call instruction? Limit return value to Ref<Value>.

                default: {
                    throw std::runtime_error("Unrecognized return instruction value");
                }
            }
        }
    }

    void TypeCheckPass::visitStoreInst(std::shared_ptr<StoreInst> node) {
        TypeKind storeInstValueTypeKind = node->value->type->typeKind;
        std::shared_ptr<AllocaInst> targetValue = node->target;

        // Attempting to store a value with a different type than what was allocated.
        if (storeInstValueTypeKind != targetValue->type->typeKind) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::instStoreTypeMismatch)

                // TODO: Get names for types to display on the error message.
                ->formatMessage("pending", "pending")

                ->setSourceLocation(node->sourceLocation)
                ->finish();
        }
    }

    void TypeCheckPass::visitStructDefinition(std::shared_ptr<StructDefinition> node) {
        if (node->values.size() != node->type->fields->getSize()) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::structWrongValueCount)

                ->formatMessage(
                    node->type->name,
                    node->values.size(),
                    node->type->fields->getSize()
                )

                ->finish();
        }
        else {
            auto declarationNativeFieldsMap = node->type->fields->unwrap();
            size_t index = 0;

            for (const auto& [name, type] : declarationNativeFieldsMap) {
                auto value = node->values[index];

                if (!value->type->isSameAs(type)) {
                    this->context->diagnosticBuilder
                        ->bootstrap(diagnostic::structIncompatibleValueType)
                        ->formatMessage(node->type->name, "pending", index, "pending")
                        ->finish();
                }

                index++;
            }
        }
    }

    void TypeCheckPass::visitCallInst(std::shared_ptr<CallInst> node) {
        if (node->callee->constructKind == ConstructKind::Function) {
            std::shared_ptr<Function> calleeFunction = node->callee->dynamicCast<Function>();
            size_t expectedArgumentCount = calleeFunction->prototype->args->items->getSize();
            size_t actualArgumentCount = node->args.size();

            if (actualArgumentCount != expectedArgumentCount) {
                this->context->diagnosticBuilder
                    ->bootstrap(diagnostic::instCallWrongArgCount)

                    ->formatMessage(
                        calleeFunction->prototype->name,
                        expectedArgumentCount,
                        actualArgumentCount
                    )

                    ->finish();
            }
            else {
                size_t index = 0;

                auto functionDeclarationArgumentsNativeMap =
                    calleeFunction->prototype->args->items->unwrap();

                for (const auto& [name, type] : functionDeclarationArgumentsNativeMap) {
                    if (node->args[index]->constructKind == ConstructKind::Type) {
                        std::shared_ptr<Type> callArgumentType =
                            node->args[index]->dynamicCast<Type>();

                        if (!callArgumentType->isSameAs(type.first)) {
                            this->context->diagnosticBuilder
                                ->bootstrap(diagnostic::instCallIncompatibleArg)

                                ->formatMessage(
                                    calleeFunction->prototype->name,
                                    type.first->name,
                                    callArgumentType->name
                                )

                                ->finish();
                        }
                    }

                    // TODO: Other construct types?

                    index++;
                }
            }
        }

        // TODO: Extern, method, etc.
    }
}
