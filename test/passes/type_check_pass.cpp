#include <ionir/passes/type_system/type_check_pass.h>
#include <ionir/misc/bootstrap.h>
#include <ionir/syntax/ast_builder.h>
#include <ionir/test/const.h>
#include "../pch.h"

using namespace ionir;

// TODO: Separate into multiple tests.
TEST(TypeCheckPassTest, Run) {
    PassManager passManager = PassManager();

    ionshared::Ptr<ionshared::PassContext> passContext =
        std::make_shared<ionshared::PassContext>();

    passManager.registerPass(std::make_shared<TypeCheckPass>(passContext));

    Ast ast = Bootstrap::functionAst(test::constant::foobar);
    ionshared::OptPtr<Function> function = ast[0]->dynamicCast<Module>()->lookupFunction(test::constant::foobar);

    EXPECT_TRUE(ionshared::util::hasValue(function));

    ionshared::Ptr<Prototype> prototype = function->get()->prototype;

    prototype->returnType = TypeFactory::typeInteger(IntegerKind::Int32);

    // TODO: For now it's throwing, but soon instead check for produced semantic error.

    /**
     * The bootstrapped function's entry section does not contain
     * any instructions. Since at least a single terminal instruction
     * is required, the pass should report a semantic error.
     */
//    EXPECT_THROW(passManager.run(ast), std::runtime_error);

    passManager.run(ast);

    EXPECT_EQ(passContext->diagnostics->getSize(), 1);

    ionshared::Diagnostic functionReturnValueMissingDiagnostic =
        (*passContext->diagnostics.get())[0];

    EXPECT_EQ(
        functionReturnValueMissingDiagnostic.code.value(),
        diagnostic::functionReturnValueMissing.code.value()
    );

    prototype->returnType = TypeFactory::typeVoid();

    ionshared::Ptr<BasicBlock> entrySection = *function->get()->body->findEntryBasicBlock();
    InstBuilder instBuilder = InstBuilder(entrySection);

    instBuilder.createReturn();

    // TODO: Compare diagnostics instead.
    /**
     * After setting the bootstrapped function's prototype's return
     * type to void and inserting a return instruction, the pass
     * should no longer complain.
     */
    EXPECT_NO_THROW(passManager.run(ast));
}

TEST(TypeCheckPassTast, FunctionReturnTypeValueMismatch) {
    ionshared::Ptr<ionshared::PassContext> passContext =
        std::make_shared<ionshared::PassContext>();

    ionshared::Ptr<TypeCheckPass> typeCheckPass = std::make_shared<TypeCheckPass>(passContext);

    Ast ast = Bootstrap::functionAst(test::constant::foobar);
    ionshared::OptPtr<Function> function = ast[0]->dynamicCast<Module>()->lookupFunction(test::constant::foobar);

    EXPECT_TRUE(function.has_value());

    ionshared::Ptr<Prototype> prototype = function->get()->prototype;

    prototype->returnType = TypeFactory::typeInteger(IntegerKind::Int32);

    ionshared::Ptr<BasicBlock> entrySection = *function->get()->body->findEntryBasicBlock();
    InstBuilder instBuilder = InstBuilder(entrySection);

    ionshared::Ptr<ReturnInst> returnInst = instBuilder.createReturn(
        std::make_shared<IntegerLiteral>(
            std::make_shared<IntegerType>(IntegerKind::Int8),
            2
        )
    );

	typeCheckPass->visitReturnInst(returnInst);

	EXPECT_EQ(passContext->diagnostics->getSize(), 1);

	ionshared::Diagnostic functionReturnValueTypeMismatchDiagnostic =
		(*passContext->diagnostics.get())[0];

	EXPECT_EQ(
        functionReturnValueTypeMismatchDiagnostic.code.value(),
        diagnostic::functionReturnValueTypeMismatch.code.value()
    );
}
