#pragma once

#include <optional>
#include <stack>
#include <queue>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/BasicBlock.h>
#include <ionshared/container/llvm_stack.h>
#include <ionshared/container/lowering_symbol_table.h>
#include <ionir/construct/value/integer_literal.h>
#include <ionir/construct/value/char_literal.h>
#include <ionir/construct/value/string_literal.h>
#include <ionir/construct/inst.h>
#include <ionir/construct/inst/branch.h>
#include <ionir/construct/inst/alloca.h>
#include <ionir/construct/inst/return.h>
#include <ionir/construct/construct.h>
#include <ionir/construct/function.h>
#include <ionir/construct/type.h>
#include <ionir/construct/function_body.h>
#include <ionir/construct/extern.h>
#include <ionir/construct/prototype.h>
#include <ionir/construct/global.h>
#include <ionir/tracking/context.h>
#include <ionir/llvm/llvm_emitted_entities.h>
#include <ionir/passes/pass.h>

namespace ionir {
    class LlvmLoweringPass : public Pass {
    private:
        struct LlvmBuffers {
            ionshared::Stack<std::shared_ptr<llvm::Module>> modules{};

            ionshared::Stack<std::shared_ptr<llvm::Function>> functions{};

            ionshared::Stack<std::shared_ptr<llvm::BasicBlock>> basicBlocks{};

            /**
             * Create a new LLVM IR builder instance and return it.
             * Will throw a runtime exception if the basic block buffer
             * is empty.
             */
            llvm::IRBuilder<> makeBuilder();
        };

        static std::shared_ptr<llvm::Type> processTypeQualifiers(
            const std::shared_ptr<TypeQualifiers>& qualifiers,
            llvm::Type* type
        );

        std::shared_ptr<ionshared::SymbolTable<llvm::Module*>> modules;

        LlvmBuffers llvmBuffers;

        ionshared::LoweringSymbolTable<std::shared_ptr<Construct>, std::shared_ptr<llvm::Value>> valueSymbolTable;

        ionshared::LoweringSymbolTable<std::shared_ptr<Construct>, std::shared_ptr<llvm::Type>> typeSymbolTable;

        /**
         * Visit and emit a construct if it has not been already
         * previously visited and emitted, and return the resulting
         * lowered construct by looking it up on the local symbol table.
         * By default, buffers are stashed during visitation of the
         * construct and restored afterwards to avoid being overwritten.
         */
        template<typename T = llvm::Value>
            requires std::derived_from<T, llvm::Value>
        std::shared_ptr<T> valueSafeEarlyVisitOrLookup(
            std::shared_ptr<Construct> construct,
            bool useDynamicCast = true,
            bool stashBuffers = true
        ) {
            // TODO: If the root (module) hasn't been emitted, emit it early. That would also emit the target along the way.

            /**
             * NOTE: If specified, buffers should be stashed and restored after
             * visitation, as the construct could be anything, including a block
             * or a function, which, among others, alter buffers when being lowered.
             */
            if (stashBuffers) {
                this->stashBuffers([&, this] {
                    /**
                     * NOTE: Construct is protected from being emitted more
                     * than once during this call.
                     */
                    this->visit(construct);
                });
            }
            else {
                this->visit(construct);
            }

            if (!this->valueSymbolTable.contains(construct)) {
                throw std::runtime_error("Visiting construct did not create an entry in the local symbol table");
            }

            return *this->valueSymbolTable.find<T>(construct, useDynamicCast);
        }

        /**
         * Visit and emit a construct if it has not been already
         * previously visited and emitted, and return the resulting
         * lowered construct by looking it up on the local symbol table.
         * By default, buffers are stashed during visitation of the
         * construct and restored afterwards to avoid being overwritten.
         */
        template<typename T = llvm::Type>
            requires std::derived_from<T, llvm::Type>
        std::shared_ptr<T> typeSafeEarlyVisitOrLookup(
            std::shared_ptr<Construct> construct,
            bool useDynamicCast = true,
            bool stashBuffers = true
        ) {
            /**
             * NOTE: If specified, buffers should be stashed and restored after
             * visitation, as the construct could be anything, including a block
             * or a function, which, among others, alter buffers when being lowered.
             */
            if (stashBuffers) {
                this->stashBuffers([&, this] {
                    /**
                 * NOTE: Construct is protected from being emitted more
                 * than once during this call.
                 */
                        this->visit(construct);
                    }
                );
            }
            else {
                this->visit(construct);
            }

            if (!this->typeSymbolTable.contains(construct)) {
                throw std::runtime_error("Visiting construct did not create an entry in the local symbol table");
            }

            return *this->typeSymbolTable.find<T>(construct, useDynamicCast);
        }

    public:
        IONSHARED_PASS_ID;

        explicit LlvmLoweringPass(
            std::shared_ptr<ionshared::PassContext> context,

            std::shared_ptr<ionshared::SymbolTable<llvm::Module*>> modules =
                std::make_shared<ionshared::SymbolTable<llvm::Module*>>()
        ) noexcept;

        ~LlvmLoweringPass();

        [[nodiscard]] std::shared_ptr<ionshared::SymbolTable<llvm::Module*>> getModules() const;

        [[nodiscard]] std::optional<llvm::Module*> getModuleBuffer() const;

        bool setModuleBuffer(const std::string& id);

        void visit(std::shared_ptr<Construct> node) override;

        bool initialize(ionshared::PassInfo& info) override;

        void finish() override;

        void visitScopeAnchor(std::shared_ptr<ScopeAnchor<>> node) override;

        void visitFunction(std::shared_ptr<Function> construct) override;

        void visitExtern(std::shared_ptr<Extern> construct) override;

        void visitBasicBlock(std::shared_ptr<BasicBlock> construct) override;

        void visitFunctionBody(std::shared_ptr<FunctionBody> node) override;

        void visitPrototype(std::shared_ptr<Prototype> construct) override;

        void visitIntegerLiteral(std::shared_ptr<IntegerLiteral> construct) override;

        void visitCharLiteral(std::shared_ptr<CharLiteral> construct) override;

        void visitStringLiteral(std::shared_ptr<StringLiteral> construct) override;

        void visitBooleanLiteral(std::shared_ptr<BooleanLiteral> node) override;

        void visitOperationValue(std::shared_ptr<OperationValue> node) override;

        void visitAllocaInst(std::shared_ptr<AllocaInst> construct) override;

        void visitReturnInst(std::shared_ptr<ReturnInst> node) override;

        void visitBranchInst(std::shared_ptr<BranchInst> node) override;

        void visitCallInst(std::shared_ptr<CallInst> node) override;

        void visitStoreInst(std::shared_ptr<StoreInst> node) override;

        void visitJumpInst(std::shared_ptr<JumpInst> node) override;

        void visitGlobal(std::shared_ptr<Global> construct) override;

        void visitIntegerType(std::shared_ptr<IntegerType> construct) override;

        void visitBooleanType(std::shared_ptr<BooleanType> construct) override;

        void visitVoidType(std::shared_ptr<VoidType> construct) override;

        void visitModule(std::shared_ptr<Module> node) override;

        void visitStruct(std::shared_ptr<Struct> construct) override;

        void visitStructDefinition(std::shared_ptr<StructDefinition> construct) override;
    };
}
