#pragma once

#include <ionshared/passes/base_pass.h>
#include <ionshared/passes/base_pass_manager.h>
#include <ionir/diagnostics/diagnostic.h>
#include <ionir/construct/construct.h>
#include <ionir/construct/value/integer_literal.h>
#include <ionir/construct/value/char_literal.h>
#include <ionir/construct/value/string_literal.h>
#include <ionir/construct/value/boolean_literal.h>
#include <ionir/construct/instruction/alloca.h>
#include <ionir/construct/instruction/branch.h>
#include <ionir/construct/instruction/return.h>
#include <ionir/construct/instruction/store.h>
#include <ionir/construct/instruction/call.h>
#include <ionir/construct/instruction/jump.h>
#include <ionir/construct/instruction/compare.h>
#include <ionir/construct/value/operation.h>
#include <ionir/construct/type/integer_type.h>
#include <ionir/construct/type/void_type.h>
#include <ionir/construct/type/boolean_type.h>
#include <ionir/construct/type/pointer_type.h>
#include <ionir/construct/pseudo/directive.h>
#include <ionir/construct/pseudo/error_marker.h>
#include <ionir/construct/function.h>
#include <ionir/construct/prototype.h>
#include <ionir/construct/extern.h>
#include <ionir/construct/type.h>
#include <ionir/construct/global.h>
#include <ionir/construct/module.h>
#include <ionir/construct/value.h>
#include <ionir/construct/instruction.h>
#include <ionir/construct/identifier.h>
#include <ionir/construct/type/struct.h>
#include <ionir/construct/value/struct_definition.h>

#define IONIR_PASS_INTERNAL_ASSERT(condition) \
    if (!this->context->diagnosticBuilder->internalAssert(condition)) { return; }

namespace ionir {
    struct Pass : ionshared::BasePass<Construct> {
        explicit Pass(std::shared_ptr<ionshared::PassContext> context) noexcept;

        void visit(std::shared_ptr<Construct> construct) override;

        virtual void visitChildren(std::shared_ptr<Construct> construct);

        virtual void visitFunction(std::shared_ptr<Function> construct);

        virtual void visitExtern(std::shared_ptr<Extern> construct);

        virtual void visitBasicBlock(std::shared_ptr<BasicBlock> construct);

        virtual void visitPrototype(std::shared_ptr<Prototype> construct);

        virtual void visitIntegerLiteral(std::shared_ptr<IntegerLiteral> construct);

        virtual void visitCharLiteral(std::shared_ptr<CharLiteral> construct);

        virtual void visitStringLiteral(std::shared_ptr<StringLiteral> construct);

        virtual void visitBooleanLiteral(std::shared_ptr<BooleanLiteral> construct);

        virtual void visitOperationValue(std::shared_ptr<OperationValue> construct);

        virtual void visitAllocaInst(std::shared_ptr<AllocaInst> construct);

        virtual void visitReturnInst(std::shared_ptr<ReturnInst> construct);

        virtual void visitBranchInst(std::shared_ptr<BranchInst> construct);

        virtual void visitCallInst(std::shared_ptr<CallInst> construct);

        virtual void visitStoreInst(std::shared_ptr<StoreInst> construct);

        virtual void visitJumpInst(std::shared_ptr<JumpInst> construct);

        virtual void visitCompareInst(std::shared_ptr<CompareInst> construct);

        virtual void visitGlobal(std::shared_ptr<Global> construct);

        virtual void visitIntegerType(std::shared_ptr<IntegerType> construct);

        virtual void visitVoidType(std::shared_ptr<VoidType> construct);

        virtual void visitBooleanType(std::shared_ptr<BooleanType> construct);

        virtual void visitPointerType(std::shared_ptr<PointerType> construct);

        virtual void visitModule(std::shared_ptr<Module> construct);

        virtual void visitDirective(Directive construct);

        virtual void visitScopeAnchor(std::shared_ptr<ScopeAnchor<>> construct);

        virtual void visitErrorMarker(std::shared_ptr<ErrorMarker> construct);

        virtual void visitIdentifier(std::shared_ptr<Identifier> construct);

        virtual void visitStructType(std::shared_ptr<StructType> construct);

        virtual void visitStructDefinition(std::shared_ptr<StructDefinition> construct);
    };

    typedef ionshared::BasePassManager<Pass, Construct> PassManager;
}
