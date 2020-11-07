#include <ionir/passes/pass.h>

namespace ionir {
    Pass::Pass(std::shared_ptr<ionshared::PassContext> context) noexcept :
        ionshared::BasePass<Construct>(std::move(context)) {
        //
    }

    void Pass::visit(std::shared_ptr<Construct> construct) {
        construct->accept(*this);
        this->visitChildren(construct);
    }

    void Pass::visitChildren(std::shared_ptr<Construct> construct) {
        // TODO: Will it cause StackOverflow error with large ASTs?
        Ast children = construct->getChildrenNodes();

        /**
         * After visiting the construct, attempt to
         * visit all its children as well.
         */
        for (const auto& child : children) {
            // TODO: CRITICAL: What if 'child' (AstNode) is not boxed under Construct?
            this->visit(child);
        }
    }

    void Pass::visitFunction(std::shared_ptr<Function> construct) {
        //
    }

    void Pass::visitExtern(std::shared_ptr<Extern> construct) {
        //
    }

    void Pass::visitBasicBlock(std::shared_ptr<BasicBlock> construct) {
        //
    }

    void Pass::visitPrototype(std::shared_ptr<Prototype> construct) {
        //
    }

    void Pass::visitIntegerLiteral(std::shared_ptr<IntegerLiteral> construct) {
        //
    }

    void Pass::visitCharLiteral(std::shared_ptr<CharLiteral> construct) {
        //
    }

    void Pass::visitStringLiteral(std::shared_ptr<StringLiteral> construct) {
        //
    }

    void Pass::visitBooleanLiteral(std::shared_ptr<BooleanLiteral> construct) {
        //
    }

    void Pass::visitAllocaInst(std::shared_ptr<AllocaInst> construct) {
        //
    }

    void Pass::visitReturnInst(std::shared_ptr<ReturnInst> construct) {
        //
    }

    void Pass::visitBranchInst(std::shared_ptr<BranchInst> construct) {
        //
    }

    void Pass::visitCallInst(std::shared_ptr<CallInst> construct) {
        //
    }

    void Pass::visitStoreInst(std::shared_ptr<StoreInst> construct) {
        //
    }

    void Pass::visitJumpInst(std::shared_ptr<JumpInst> construct) {
        //
    }

    void Pass::visitCompareInst(std::shared_ptr<CompareInst> construct) {
        //
    }

    void Pass::visitOperationValue(std::shared_ptr<OperationValue> construct) {
        //
    }

    void Pass::visitGlobal(std::shared_ptr<Global> construct) {
        //
    }

    void Pass::visitIntegerType(std::shared_ptr<IntegerType> construct) {
        //
    }

    void Pass::visitVoidType(std::shared_ptr<VoidType> construct) {
        //
    }

    void Pass::visitBooleanType(std::shared_ptr<BooleanType> construct) {
        //
    }

    void Pass::visitPointerType(std::shared_ptr<PointerType> construct) {
        //
    }

    void Pass::visitModule(std::shared_ptr<Module> construct) {
        //
    }

    void Pass::visitDirective(Directive construct) {
        //
    }

    void Pass::visitScopeAnchor(std::shared_ptr<ScopeAnchor<>> construct) {
        //
    }

    void Pass::visitErrorMarker(std::shared_ptr<ErrorMarker> construct) {
        //
    }

    void Pass::visitIdentifier(std::shared_ptr<Identifier> construct) {
        //
    }

    void Pass::visitStructType(std::shared_ptr<StructType> construct) {
        //
    }

    void Pass::visitStructDefinition(std::shared_ptr<StructDefinition> construct) {
        //
    }

    void Pass::visitMethod(std::shared_ptr<Method> construct) {
        //
    }
}
