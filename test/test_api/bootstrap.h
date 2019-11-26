#pragma once

#include <array>
#include <vector>
#include "syntax/token.h"
#include "misc/iterable.h"
#include "parsing/parser.h"
#include "llvm/module.h"
#include "llvm/context.h"
#include "code_gen/llvm_visitor.h"
#include "util.h"

namespace ionir::test::bootstrap
{
Token token(int id);

TokenStream tokenStream(int amountOfItems = 1);

Parser parser(std::vector<Token> tokens);

Module *module(std::string identifier = "test");

LlvmVisitor *llvmVisitor();
} // namespace ionir::test::bootstrap
