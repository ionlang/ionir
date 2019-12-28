#pragma once

#include <optional>
#include <stack>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/BasicBlock.h>
#include <ionir/construct/values/integer.h>
#include <ionir/construct/values/char.h>
#include <ionir/construct/values/string.h>
#include <ionir/construct/expr/binary_expr.h>
#include <ionir/construct/insts/inst.h>
#include <ionir/construct/insts/branch.h>
#include <ionir/construct/insts/alloca.h>
#include <ionir/construct/insts/return.h>
#include <ionir/construct/construct.h>
#include <ionir/construct/function.h>
#include <ionir/construct/type.h>
#include <ionir/construct/block.h>
#include <ionir/construct/extern.h>
#include <ionir/construct/prototype.h>
#include <ionir/construct/global.h>
#include <ionir/container/stack.h>
#include <ionir/misc/helpers.h>
#include <ionir/passes/pass.h>

namespace ionir {
    class LlvmVisitor : public Pass {
    protected:
        llvm::LLVMContext *context;

        llvm::Module *module;

        Stack<llvm::Value *> valueStack;

        Stack<llvm::Type *> typeStack;

        std::optional<llvm::Function *> function;

        std::optional<llvm::IRBuilder<>> builder;

        std::map<std::string, llvm::Value *> namedValues;

        Stack<llvm::IRBuilder<>> builderTracker;

        /**
         * Ensure that the builder is instantiated, otherwise
         * throws a runtime error.
         */
        void requireBuilder();

        void requireFunction();

        void setBuilder(llvm::BasicBlock *block);

        bool saveBuilder();

        bool restoreBuilder();

    public:
        explicit LlvmVisitor(llvm::Module *module);

        llvm::Module *getModule() const;

        Stack<llvm::Value *> getValueStack() const;

        Stack<llvm::Type *> getTypeStack() const;

        void visitFunction(Ptr<Function> node) override;

        void visitExtern(Ptr<Extern> node) override;

        void visitSection(Ptr<Section> node) override;

        void visitBlock(Ptr<Block> node) override;

        void visitType(Ptr<Type> node) override;

        void visitPrototype(Ptr<Prototype> node) override;

        void visitBinaryExpr(Ptr<BinaryExpr> node) override;

        void visitIntegerValue(Ptr<IntegerValue> node) override;

        void visitCharValue(Ptr<CharValue> node) override;

        void visitStringValue(Ptr<StringValue> node) override;

        void visitBooleanValue(Ptr<BooleanValue> node) override;

        void visitAllocaInst(Ptr<AllocaInst> node) override;

        void visitReturnInst(Ptr<ReturnInst> node) override;

        void visitBranchInst(Ptr<BranchInst> node) override;

        void visitGlobal(Ptr<Global> node) override;
    };
}