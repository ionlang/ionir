#pragma once

#include <ionshared/passes/base_pass_manager.h>
#include <ionshared/passes/base_pass.h>
#include <ionir/construct/intrinsic/reflection/name_of.h>
#include <ionir/construct/intrinsic/reflection/type_of.h>
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
#include <ionir/construct/instruction/cast.h>
#include <ionir/construct/value/operation.h>
#include <ionir/construct/value/struct_definition.h>
#include <ionir/construct/type/integer_type.h>
#include <ionir/construct/type/decimal_type.h>
#include <ionir/construct/type/void_type.h>
#include <ionir/construct/type/boolean_type.h>
#include <ionir/construct/type/pointer_type.h>
#include <ionir/construct/type/struct.h>
#include <ionir/construct/pseudo/directive.h>
#include <ionir/construct/pseudo/error_marker.h>
#include <ionir/construct/pseudo/comment.h>
#include <ionir/construct/function.h>
#include <ionir/construct/prototype.h>
#include <ionir/construct/extern.h>
#include <ionir/construct/type.h>
#include <ionir/construct/global.h>
#include <ionir/construct/module.h>
#include <ionir/construct/value.h>
#include <ionir/construct/instruction.h>
#include <ionir/construct/identifier.h>
#include <ionir/construct/method.h>
#include <ionir/construct/destructor.h>
#include <ionir/diagnostics/diagnostic.h>
#include <ionir/construct/construct.h>

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

        virtual void visitIntegerLiteral(std::shared_ptr<LiteralInteger> construct);

        virtual void visitCharLiteral(std::shared_ptr<LiteralChar> construct);

        virtual void visitStringLiteral(std::shared_ptr<StringLiteral> construct);

        virtual void visitBooleanLiteral(std::shared_ptr<LiteralBoolean> construct);

        virtual void visitOperationValue(std::shared_ptr<OperationValue> construct);

        virtual void visitAllocaInst(std::shared_ptr<InstAlloca> construct);

        virtual void visitReturnInst(std::shared_ptr<InstReturn> construct);

        virtual void visitBranchInst(std::shared_ptr<InstBranch> construct);

        virtual void visitCallInst(std::shared_ptr<InstCall> construct);

        virtual void visitStoreInst(std::shared_ptr<InstStore> construct);

        virtual void visitJumpInst(std::shared_ptr<InstJump> construct);

        virtual void visitCompareInst(std::shared_ptr<InstCompare> construct);

        virtual void visitCastInst(std::shared_ptr<InstCast> construct);

        virtual void visitGlobal(std::shared_ptr<Global> construct);

        virtual void visitIntegerType(std::shared_ptr<TypeInteger> construct);

        virtual void visitDecimalType(std::shared_ptr<TypeDecimal> construct);

        virtual void visitVoidType(std::shared_ptr<TypeVoid> construct);

        virtual void visitBooleanType(std::shared_ptr<TypeBoolean> construct);

        virtual void visitPointerType(std::shared_ptr<TypePointer> construct);

        virtual void visitModule(std::shared_ptr<Module> construct);

        virtual void visitDirective(Directive construct);

        virtual void visitScopeAnchor(std::shared_ptr<ScopeAnchor<>> construct);

        virtual void visitErrorMarker(std::shared_ptr<ErrorMarker> construct);

        virtual void visitIdentifier(std::shared_ptr<Identifier> construct);

        virtual void visitStructType(std::shared_ptr<TypeStruct> construct);

        virtual void visitStructDefinition(std::shared_ptr<StructDefinition> construct);

        virtual void visitMethod(std::shared_ptr<Method> construct);

        virtual void visitNameOfIntrinsic(std::shared_ptr<NameOfIntrinsic> construct);

        virtual void visitTypeOfIntrinsic(std::shared_ptr<TypeOfIntrinsic> construct);

        virtual void visitComment(std::shared_ptr<Comment> construct);

        virtual void visitConstructor(std::shared_ptr<Constructor> construct);

        virtual void visitDestructor(std::shared_ptr<Destructor> construct);
    };

    typedef ionshared::BasePassManager<Pass, Construct> PassManager;
}
