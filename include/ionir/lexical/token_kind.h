#pragma once

#include <iostream>

namespace ionir {
    enum class TokenKind {
        Dummy,

        Unknown,

        Identifier,

        LiteralString,

        LiteralDecimal,

        LiteralInt,

        LiteralCharacter,

        Whitespace,

        SymbolAt,

        SymbolColon,

        SymbolDollar,

        SymbolHash,

        SymbolParenthesesL,

        SymbolParenthesesR,

        SymbolBracketL,

        SymbolBracketR,

        SymbolComma,

        SymbolTilde,

        SymbolEqual,

        SymbolSemiColon,

        SymbolStar,

        SymbolBraceL,

        SymbolBraceR,

        SymbolArrow,

        InstCall,

        InstStore,

        InstReturn,

        InstAlloca,

        InstBranch,

        KeywordFunction,

        KeywordExtern,

        KeywordElse,

        KeywordGlobal,

        KeywordMutable,

        KeywordModule,

        TypeVoid,

        TypeString,

        TypeInt16,

        TypeInt32,

        TypeInt64,

        OperatorAdd,

        OperatorSub,

        OperatorMultiply,

        OperatorDivide,

        OperatorModulo,

        OperatorExponent,

        OperatorGreaterThan,

        OperatorLessThan
    };

    std::ostream &operator<<(std::ostream &stream, const TokenKind &tokenKind);
}