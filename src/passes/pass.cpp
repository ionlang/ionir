#include <ionir/passes/pass.h>

namespace ionir {
    Pass::Pass(std::shared_ptr<ionshared::PassContext> context) noexcept :
        ionshared::BasePass<Construct>(std::move(context)) {
        //
    }

    void Pass::visit(std::shared_ptr<Construct> node) {
        node->accept(*this);
        this->visitChildren(node);
    }

    void Pass::visitChildren(std::shared_ptr<Construct> node) {
        // TODO: Will it cause StackOverflow error with large ASTs?
        Ast children = node->getChildrenNodes();

        /**
         * After visiting the node, attempt to
         * visit all its children as well.
         */
        for (const auto& child : children) {
            // TODO: CRITICAL: What if 'child' (AstNode) is not boxed under Construct?
            this->visit(child);
        }
    }

    void Pass::visitFunction(std::shared_ptr<Function> node) {
        //
    }

    void Pass::visitExtern(std::shared_ptr<Extern> node) {
        //
    }

    void Pass::visitBasicBlock(std::shared_ptr<BasicBlock> node) {
        //
    }

    void Pass::visitPrototype(std::shared_ptr<Prototype> node) {
        //
    }

    void Pass::visitIntegerLiteral(std::shared_ptr<IntegerLiteral> node) {
        //
    }

    void Pass::visitCharLiteral(std::shared_ptr<CharLiteral> node) {
        //
    }

    void Pass::visitStringLiteral(std::shared_ptr<StringLiteral> node) {
        //
    }

    void Pass::visitBooleanLiteral(std::shared_ptr<BooleanLiteral> node) {
        //
    }

    void Pass::visitAllocaInst(std::shared_ptr<AllocaInst> node) {
        //
    }

    void Pass::visitReturnInst(std::shared_ptr<ReturnInst> node) {
        //
    }

    void Pass::visitBranchInst(std::shared_ptr<BranchInst> node) {
        //
    }

    void Pass::visitCallInst(std::shared_ptr<CallInst> node) {
        //
    }

    void Pass::visitStoreInst(std::shared_ptr<StoreInst> node) {
        //
    }

    void Pass::visitJumpInst(std::shared_ptr<JumpInst> node) {
        //
    }

    void Pass::visitCompareInst(std::shared_ptr<CompareInst> construct) {
        //
    }

    void Pass::visitOperationValue(std::shared_ptr<OperationValue> node) {
        //
    }

    void Pass::visitGlobal(std::shared_ptr<Global> node) {
        //
    }

    void Pass::visitIntegerType(std::shared_ptr<IntegerType> node) {
        //
    }

    void Pass::visitVoidType(std::shared_ptr<VoidType> node) {
        //
    }

    void Pass::visitBooleanType(std::shared_ptr<BooleanType> node) {
        //
    }

    void Pass::visitPointerType(std::shared_ptr<PointerType> node) {
        //
    }

    void Pass::visitModule(std::shared_ptr<Module> node) {
        //
    }

    void Pass::visitDirective(Directive node) {
        //
    }

    void Pass::visitScopeAnchor(std::shared_ptr<ScopeAnchor<>> node) {
        //
    }

    void Pass::visitErrorMarker(std::shared_ptr<ErrorMarker> node) {
        //
    }

    void Pass::visitIdentifier(std::shared_ptr<Identifier> node) {
        //
    }

    void Pass::visitStruct(std::shared_ptr<Struct> node) {
        //
    }

    void Pass::visitStructDefinition(std::shared_ptr<StructDefinition> node) {
        //
    }
}
