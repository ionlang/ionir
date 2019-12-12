#pragma once

#include <memory>
#include <optional>
#include <stack>
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "code_gen/node.h"
#include "code_gen/node_kind.h"
#include "ast_nodes/values/integer.h"
#include "ast_nodes/values/char.h"
#include "ast_nodes/values/string.h"
#include "ast_nodes/type.h"
#include "ast_nodes/block.h"
#include "ast_nodes/binary_expr.h"
#include "ast_nodes/function.h"
#include "ast_nodes/extern.h"
#include "ast_nodes/prototype.h"
#include "ast_nodes/global_var.h"
#include "ast_nodes/inst.h"
#include "ast_nodes/insts/branch.h"
#include "ast_nodes/insts/alloca.h"
#include "ast_nodes/insts/return.h"
#include "misc/stack.h"
#include "passes/pass.h"

namespace ionir
{
class LlvmVisitor : public Pass
{
protected:
	llvm::LLVMContext *context;

	llvm::Module *module;

	Stack<llvm::Value *> valueStack;

	Stack<llvm::Type *> typeStack;

	std::optional<llvm::Function *> function;

	std::optional<llvm::IRBuilder<>> builder;

	std::map<std::string, llvm::Value *> namedValues;

	/**
	 * Ensures that the builder is instantiated, otherwise
	 * throws a runtime error.
	 */
	void requireBuilder();

	void requireFunction();

public:
	LlvmVisitor(llvm::Module *module);

	~LlvmVisitor();

	llvm::Module *getModule() const;

	Stack<llvm::Value *> getValueStack() const;

	Stack<llvm::Type *> getTypeStack() const;

	std::shared_ptr<Node> visitFunction(std::shared_ptr<Function> node) override;

	std::shared_ptr<Node> visitExtern(std::shared_ptr<Extern> node) override;

	std::shared_ptr<Node> visitSection(std::shared_ptr<Section> node) override;

	std::shared_ptr<Node> visitBlock(std::shared_ptr<Block> node) override;

	std::shared_ptr<Node> visitType(std::shared_ptr<Type> node) override;

	std::shared_ptr<Node> visitBinaryExpr(std::shared_ptr<BinaryExpr> node) override;

	std::shared_ptr<Node> visitPrototype(std::shared_ptr<Prototype> node) override;

	std::shared_ptr<Node> visitIntValue(std::shared_ptr<IntValue> node) override;

	std::shared_ptr<Node> visitCharValue(std::shared_ptr<CharValue> node) override;

	std::shared_ptr<Node> visitStringValue(std::shared_ptr<StringValue> node) override;

	std::shared_ptr<Node> visitAllocaInst(std::shared_ptr<AllocaInst> node) override;

	std::shared_ptr<Node> visitReturnInst(std::shared_ptr<ReturnInst> node) override;

	std::shared_ptr<Node> visitBranchInst(std::shared_ptr<BranchInst> node) override;

	std::shared_ptr<Node> visitGlobalVar(std::shared_ptr<GlobalVar> node) override;
};
} // namespace ionir
