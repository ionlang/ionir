#pragma once

#include <ionshared/passes/base_pass.h>
#include <ionshared/passes/base_pass_manager.h>
#include <ionir/diagnostics/diagnostic.h>
#include <ionir/construct/construct.h>
#include <ionir/construct/value/integer_literal.h>
#include <ionir/construct/value/char_literal.h>
#include <ionir/construct/value/string_literal.h>
#include <ionir/construct/value/boolean_literal.h>
#include <ionir/construct/inst/alloca.h>
#include <ionir/construct/inst/branch.h>
#include <ionir/construct/inst/return.h>
#include <ionir/construct/inst/store.h>
#include <ionir/construct/inst/call.h>
#include <ionir/construct/inst/jump.h>
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
#include <ionir/construct/function_body.h>
#include <ionir/construct/type.h>
#include <ionir/construct/global.h>
#include <ionir/construct/module.h>
#include <ionir/construct/value.h>
#include <ionir/construct/inst.h>
#include <ionir/construct/identifier.h>
#include <ionir/construct/struct.h>
#include <ionir/construct/struct_definition.h>

#define IONIR_PASS_INTERNAL_ASSERT(condition) \
    if (!this->context->diagnosticBuilder->internalAssert(condition)) { return; }

namespace ionir {
    struct Pass : ionshared::BasePass<Construct> {
        explicit Pass(std::shared_ptr<ionshared::PassContext> context) noexcept;

        void visit(std::shared_ptr<Construct> node) override;

        virtual void visitChildren(std::shared_ptr<Construct> node);

        virtual void visitFunction(std::shared_ptr<Function> node);

        virtual void visitExtern(std::shared_ptr<Extern> node);

        virtual void visitBasicBlock(std::shared_ptr<BasicBlock> node);

        virtual void visitFunctionBody(std::shared_ptr<FunctionBody> node);

        virtual void visitPrototype(std::shared_ptr<Prototype> node);

        virtual void visitIntegerLiteral(std::shared_ptr<IntegerLiteral> node);

        virtual void visitCharLiteral(std::shared_ptr<CharLiteral> node);

        virtual void visitStringLiteral(std::shared_ptr<StringLiteral> node);

        virtual void visitBooleanLiteral(std::shared_ptr<BooleanLiteral> node);

        virtual void visitOperationValue(std::shared_ptr<OperationValue> node);

        virtual void visitAllocaInst(std::shared_ptr<AllocaInst> node);

        virtual void visitReturnInst(std::shared_ptr<ReturnInst> node);

        virtual void visitBranchInst(std::shared_ptr<BranchInst> node);

        virtual void visitCallInst(std::shared_ptr<CallInst> node);

        virtual void visitStoreInst(std::shared_ptr<StoreInst> node);

        virtual void visitJumpInst(std::shared_ptr<JumpInst> node);

        virtual void visitGlobal(std::shared_ptr<Global> node);

        virtual void visitIntegerType(std::shared_ptr<IntegerType> node);

        virtual void visitVoidType(std::shared_ptr<VoidType> node);

        virtual void visitBooleanType(std::shared_ptr<BooleanType> node);

        virtual void visitPointerType(std::shared_ptr<PointerType> node);

        virtual void visitModule(std::shared_ptr<Module> node);

        virtual void visitDirective(Directive node);

        virtual void visitScopeAnchor(std::shared_ptr<ScopeAnchor<>> node);

        virtual void visitErrorMarker(std::shared_ptr<ErrorMarker> node);

        virtual void visitIdentifier(std::shared_ptr<Identifier> node);

        virtual void visitStruct(std::shared_ptr<Struct> node);

        virtual void visitStructDefinition(std::shared_ptr<StructDefinition> node);
    };

    typedef ionshared::BasePassManager<Pass, Construct> PassManager;
}
