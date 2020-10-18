#include <ionir/test/bootstrap.h>
#include <ionir/test/const.h>
#include "../pch.h"

using namespace ionir;

//TEST(ModuleConstructTest, ParseEmpty) {
//    Parser parser = test::bootstrap::parser({
//        Token(TokenKind::KeywordModule, "module"),
//        Token(TokenKind::Identifier, test::constant::foobar),
//        Token(TokenKind::SymbolBraceL, "{"),
//        Token(TokenKind::SymbolBraceR, "}")
//    });
//
//    AstPtrResult<Module> module = parser.parseModule();
//
//    EXPECT_TRUE(util::hasValue(module));
//
//    std::shared_ptr<Module> moduleValue = util::getResultValue(module);
//
//    EXPECT_EQ(moduleValue->getId(), test::constant::foobar);
//    EXPECT_TRUE(moduleValue->getContext()->getGlobalScope()->isEmpty());
//}
