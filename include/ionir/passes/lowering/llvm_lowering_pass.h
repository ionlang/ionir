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
#include <ionir/construct/instruction.h>
#include <ionir/construct/instruction/branch.h>
#include <ionir/construct/instruction/alloca.h>
#include <ionir/construct/instruction/return.h>
#include <ionir/construct/construct.h>
#include <ionir/construct/function.h>
#include <ionir/construct/type.h>
#include <ionir/construct/extern.h>
#include <ionir/construct/prototype.h>
#include <ionir/construct/global.h>
#include <ionir/tracking/context.h>
#include <ionir/passes/pass.h>

namespace ionir {
    struct LlvmBuffers {
        ionshared::Stack<std::shared_ptr<llvm::Module>> modules{};

        ionshared::Stack<llvm::Function*> functions{};

        ionshared::Stack<llvm::BasicBlock*> basicBlocks{};

        /**
         * Create a new LLVM IR builder instance and return it.
         * Will throw a runtime exception if the basic block buffer
         * is empty.
         */
        llvm::IRBuilder<> makeBuilder();
    };

    struct LocalBuffers {
        ionshared::Stack<std::shared_ptr<Module>> modules{};
    };

    class LlvmLoweringPass : public Pass {
    private:
        static llvm::Type* processTypeQualifiers(
            const std::shared_ptr<TypeQualifiers>& qualifiers,
            llvm::Type* llvmType
        );

        ionshared::LoweringSymbolTable<
            std::shared_ptr<Module>,
            std::shared_ptr<llvm::Module>
        > moduleSymbolTable;

        ionshared::LoweringSymbolTable<
            std::shared_ptr<Construct>,
            llvm::Value*
        > valueSymbolTable;

        ionshared::LoweringSymbolTable<
            std::shared_ptr<Construct>,
            llvm::Type*
        > typeSymbolTable;

        void eagerVisit(const std::shared_ptr<Construct>& construct) {
            std::shared_ptr<Construct> rootConstruct = construct->fetchRootConstruct();

            if (rootConstruct->constructKind != ConstructKind::Module) {
                // TODO: Use diagnostics API (internal error).
                throw std::runtime_error("Expected root construct to be a module");
            }

            std::shared_ptr<Module> rootModule = rootConstruct->dynamicCast<Module>();

            /**
             * If the root construct is verified to be a module, and
             * has not been (or started being) emitted, visit it. This
             * also conveniently means that the specified construct will
             * be visited, since it is a child of the module.
             */
            if (!this->moduleSymbolTable.contains(rootModule)) {
                this->visit(rootModule);
            }
            /**
             * At this point we know that the root construct is verified
             * to be a module, which has been (or is in the process of being)
             * visited and emitted. The only two possibilities left are that the
             * construct is part of the current module being visited, in which
             * case it should simply be visited, and the other being that the
             * construct is part of another module which has previously been
             * visited and emitted entirely, in which case the construct does
             * not need to be visited because it should be already present in
             * the symbol table. If the root construct is a the same module than
             * the current module being emitted, visit the construct. If, during
             * the visitation, the construct has previously been visited, a check
             * exists to prevent it from being emitted twice. If the construct has
             * not been emitted already, it will be emitted during this visit.
             */
            else if (this->llvmBuffers.modules.forceGetTopItem()
                == *this->moduleSymbolTable.lookup<llvm::Module>(rootModule)) {
                this->visit(construct);
            }

            if (!this->valueSymbolTable.contains(construct) && !this->typeSymbolTable.contains(construct)) {
                // TODO: Use diagnostics API (internal error).
                throw std::runtime_error("Visiting construct did not create an entry in the local symbol table");
            }
        }

        /**
         * Visit and emit a construct if it has not been already
         * previously visited and emitted, and return the resulting
         * lowered construct by looking it up on the local symbol table.
         * By default, buffers are stashed during visitation of the
         * construct and restored afterwards to avoid being overwritten.
         */
        template<typename T = llvm::Value>
            requires std::derived_from<T, llvm::Value>
        T* eagerVisitValue(
            const std::shared_ptr<Construct>& construct
        ) {
            this->eagerVisit(construct);

            // TODO: Fix.
//            return *this->valueSymbolTable.find<T>(
//                construct,
//                ionshared::CastKind::DynamicPointerCast
//            );
            std::optional<llvm::Value*> lookupResult =
                this->valueSymbolTable.unwrap().lookup(construct);

            if (!ionshared::util::hasValue(lookupResult)) {
                throw std::runtime_error("Value lookup failed");
            }

            T* castResult = llvm::dyn_cast_or_null<T>(*lookupResult);

            if (castResult == nullptr) {
                throw std::runtime_error("LLVM value cast failed");
            }

            return castResult;
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
        T* eagerVisitType(
            const std::shared_ptr<Construct>& construct
        ) {
            this->eagerVisit(construct);

            // TODO: Fix.
//            return *this->typeSymbolTable.find<T>(
//                construct,
//                ionshared::CastKind::DynamicPointerCast
//            );
            std::optional<llvm::Type*> lookupResult =
                this->typeSymbolTable.unwrap().lookup(construct);

            if (!ionshared::util::hasValue(lookupResult)) {
                throw std::runtime_error("Value lookup failed");
            }

            T* castResult = llvm::dyn_cast_or_null<T>(*lookupResult);

            if (castResult == nullptr) {
                throw std::runtime_error("LLVM value cast failed");
            }

            return castResult;
        }

    public:
        IONSHARED_PASS_ID;

        ionshared::PtrSymbolTable<llvm::Module> llvmModules;

        LlvmBuffers llvmBuffers;

        LocalBuffers localBuffers;

        explicit LlvmLoweringPass(
            std::shared_ptr<ionshared::PassContext> context
        ) noexcept;

        void visit(std::shared_ptr<Construct> node) override;

        bool initialize(ionshared::PassInfo& info) override;

        void finish() override;

        void visitScopeAnchor(std::shared_ptr<ScopeAnchor<>> node) override;

        void visitFunction(std::shared_ptr<Function> construct) override;

        void visitExtern(std::shared_ptr<Extern> construct) override;

        void visitBasicBlock(std::shared_ptr<BasicBlock> construct) override;

        void visitPrototype(std::shared_ptr<Prototype> construct) override;

        void visitIntegerLiteral(std::shared_ptr<IntegerLiteral> construct) override;

        void visitCharLiteral(std::shared_ptr<CharLiteral> construct) override;

        void visitStringLiteral(std::shared_ptr<StringLiteral> construct) override;

        void visitBooleanLiteral(std::shared_ptr<BooleanLiteral> construct) override;

        void visitOperationValue(std::shared_ptr<OperationValue> construct) override;

        void visitAllocaInst(std::shared_ptr<AllocaInst> construct) override;

        void visitReturnInst(std::shared_ptr<ReturnInst> construct) override;

        void visitBranchInst(std::shared_ptr<BranchInst> construct) override;

        void visitCallInst(std::shared_ptr<CallInst> construct) override;

        void visitStoreInst(std::shared_ptr<StoreInst> construct) override;

        void visitJumpInst(std::shared_ptr<JumpInst> node) override;

        void visitGlobal(std::shared_ptr<Global> construct) override;

        void visitIntegerType(std::shared_ptr<IntegerType> construct) override;

        void visitBooleanType(std::shared_ptr<BooleanType> construct) override;

        void visitVoidType(std::shared_ptr<VoidType> construct) override;

        void visitModule(std::shared_ptr<Module> construct) override;

        void visitStructType(std::shared_ptr<StructType> construct) override;

        void visitStructDefinition(
            std::shared_ptr<StructDefinition> construct
        ) override;

        void visitNameOfIntrinsic(std::shared_ptr<NameOfIntrinsic> construct) override;

        void visitTypeOfIntrinsic(std::shared_ptr<TypeOfIntrinsic> construct) override;
    };
}
