#include <ionir/passes/type_system/type_check_pass.h>
#include <ionir/misc/bootstrap.h>
#include <ionir/syntax/ast_builder.h>
#include <ionir/test/const.h>
#include "../pch.h"

using namespace ionir;

// TODO: Separate into multiple tests.
TEST(TypeCheckPassTest, Run) {
    PassManager passManager{};

    std::shared_ptr<ionshared::PassContext> passContext =
        std::make_shared<ionshared::PassContext>();

    passManager.registerPass(std::make_shared<TypeCheckPass>(passContext));

    Ast ast = Bootstrap::functionAst(test::constant::foobar);

    std::optional<std::shared_ptr<Function>> functionLookupResult =
        ast.front()->dynamicCast<Module>()->lookup<Function>(test::constant::foobar);

    ASSERT_TRUE(ionshared::util::hasValue(functionLookupResult));

    std::shared_ptr<Function> function = *functionLookupResult;

    function->prototype->returnType =
        std::make_shared<TypeInteger>(IntegerKind::Int32);

    // TODO: For now it's throwing, but soon instead check for produced semantic error.

    /**
     * The bootstrapped function's entry section does not contain
     * any instructions. Since at least a single terminal instruction
     * is required, the pass should report a semantic error.
     */
//    EXPECT_THROW(passManager.run(ast), std::runtime_error);

    passManager.run(ast);
    ASSERT_EQ(passContext->diagnostics->getSize(), 1);

    ionshared::Diagnostic functionReturnValueMissingDiagnostic =
        (*passContext->diagnostics)[0];

    EXPECT_EQ(
        functionReturnValueMissingDiagnostic.code.value(),
        diagnostic::functionReturnValueMissing.code.value()
    );

    function->prototype->returnType =
        std::make_shared<TypeVoid>();

    // TODO: Verify that the body has at least one basic block.

    std::shared_ptr<BasicBlock> entrySection = functionLookupResult->get()->basicBlocks.front();

    InstBuilder(entrySection).makeReturn();

    // TODO: Compare diagnostics instead.
    /**
     * After setting the bootstrapped function's prototype's return
     * type to void and inserting a return instruction, the pass
     * should no longer complain.
     */
    EXPECT_NO_THROW(passManager.run(ast));
}

TEST(TypeCheckPassTest, FunctionReturnTypeValueMismatch) {
    std::shared_ptr<ionshared::PassContext> passContext =
        std::make_shared<ionshared::PassContext>();

    std::shared_ptr<TypeCheckPass> typeCheckPass =
        std::make_shared<TypeCheckPass>(passContext);

    Ast ast = Bootstrap::functionAst(test::constant::foobar);

    std::optional<std::shared_ptr<Function>> function =
        ast[0]->dynamicCast<Module>()->lookup<Function>(test::constant::foobar);

    ASSERT_TRUE(function.has_value());

    std::shared_ptr<Prototype> prototype = function->get()->prototype;

    prototype->returnType = std::make_shared<TypeInteger>(IntegerKind::Int32);

    // TODO: Verify that the body has at least one basic block.

    std::shared_ptr<BasicBlock> entrySection{function->get()->basicBlocks.begin()->get()};

    std::shared_ptr<InstReturn> returnInst = InstBuilder(entrySection).makeReturn(
        std::make_shared<LiteralInteger>(
            std::make_shared<TypeInteger>(IntegerKind::Int8),
            2
        )->flatten()
    );

    typeCheckPass->visitReturnInst(returnInst);
    EXPECT_EQ(passContext->diagnostics->getSize(), 1);

    ionshared::Diagnostic functionReturnValueTypeMismatchDiagnostic =
        (*passContext->diagnostics)[0];

    EXPECT_EQ(
        functionReturnValueTypeMismatchDiagnostic.code.value(),
        diagnostic::functionReturnValueTypeMismatch.code.value()
    );
}
