#include <ionir/passes/pass.h>

namespace ionir {
    Pass::Pass(ionshared::Ptr<ionshared::PassContext> context) noexcept :
        ionshared::BasePass<Construct>(std::move(context)) {
        //
    }

    void Pass::visit(ionshared::Ptr<Construct> node) {
        node->accept(*this);
        this->visitChildren(node);
    }

    void Pass::visitChildren(ionshared::Ptr<Construct> node) {
        // TODO: Will it cause StackOverflow error with large ASTs?
        Ast children = node->getChildrenNodes();

        /**
         * After visiting the node, attempt to
         * visit all its children as well.
         */
        for (const auto &child : children) {
            // TODO: CRITICAL: What if 'child' (AstNode) is not boxed under Construct?
            this->visit(child);
        }
    }

    void Pass::visitFunction(ionshared::Ptr<Function> node) {
        //
    }

    void Pass::visitExtern(ionshared::Ptr<Extern> node) {
        //
    }

    void Pass::visitBasicBlock(ionshared::Ptr<BasicBlock> node) {
        //
    }

    void Pass::visitFunctionBody(ionshared::Ptr<FunctionBody> node) {
        //
    }

    void Pass::visitPrototype(ionshared::Ptr<Prototype> node) {
        //
    }

    void Pass::visitVarReference(ionshared::Ptr<VariableRef> node) {
        //
    }

    void Pass::visitIntegerLiteral(ionshared::Ptr<IntegerLiteral> node) {
        //
    }

    void Pass::visitCharLiteral(ionshared::Ptr<CharLiteral> node) {
        //
    }

    void Pass::visitStringLiteral(ionshared::Ptr<StringLiteral> node) {
        //
    }

    void Pass::visitBooleanLiteral(ionshared::Ptr<BooleanLiteral> node) {
        //
    }

    void Pass::visitAllocaInst(ionshared::Ptr<AllocaInst> node) {
        //
    }

    void Pass::visitReturnInst(ionshared::Ptr<ReturnInst> node) {
        //
    }

    void Pass::visitBranchInst(ionshared::Ptr<BranchInst> node) {
        //
    }

    void Pass::visitCallInst(ionshared::Ptr<CallInst> node) {
        //
    }

    void Pass::visitStoreInst(ionshared::Ptr<StoreInst> node) {
        //
    }

    void Pass::visitJumpInst(ionshared::Ptr<JumpInst> node) {
        //
    }

    void Pass::visitOperationInst(ionshared::Ptr<OperationInst> node) {
        //
    }

    void Pass::visitGlobal(ionshared::Ptr<Global> node) {
        //
    }

    void Pass::visitStructDecl(ionshared::Ptr<StructDecl> node) {
        //
    }

    void Pass::visitIntegerType(ionshared::Ptr<IntegerType> node) {
        //
    }

    void Pass::visitVoidType(ionshared::Ptr<VoidType> node) {
        //
    }

    void Pass::visitBooleanType(ionshared::Ptr<BooleanType> node) {
        //
    }

    void Pass::visitPointerType(ionshared::Ptr<PointerType> node) {
        //
    }

    void Pass::visitModule(ionshared::Ptr<Module> node) {
        //
    }

    void Pass::visitDirective(Directive node) {
        //
    }

    void Pass::visitScopeAnchor(ionshared::Ptr<ScopeAnchor<>> node) {
        //
    }

    void Pass::visitErrorMarker(ionshared::Ptr<ErrorMarker> node) {
        //
    }

    void Pass::visitIdentifier(ionshared::Ptr<Identifier> node) {
        //
    }

    void Pass::visitStruct(ionshared::Ptr<Struct> node) {
        //
    }
}
