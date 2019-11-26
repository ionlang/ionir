#pragma once

#include "bootstrap.h"

namespace fs = std::filesystem;

namespace ionir::test::bootstrap
{
Token token(int id)
{
    return Token(TokenType::Unknown, std::string("test_" + std::to_string(id)), id);
}

TokenStream tokenStream(int amountOfItems = 1)
{
    std::vector<Token> tokens = {};

    // Populate the tokens vector.
    for (int i = 0; i < amountOfItems; i++)
    {
        tokens.push_back(token(i));
    }

    // Create the Stream and provide the tokens.
    return TokenStream(tokens);
}

Parser parser(std::vector<Token> tokens)
{
    ionir::TokenStream stream = ionir::TokenStream(tokens);

    return ionir::Parser(stream);
}

Module *module(std::string identifier = "test")
{
    llvm::LLVMContext *llvmContext = new llvm::LLVMContext();
    llvm::Module *llvmModule = new llvm::Module("test", *llvmContext);

    return new Module(llvmModule);
}

LlvmVisitor *llvmVisitor()
{
    return new LlvmVisitor(module()->unwrap());
}
} // namespace ionir::test::bootstrap
