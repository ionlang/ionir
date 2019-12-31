#pragma once

#include <string>
#include <ionir/misc/iterable.h>
#include "token_type.h"

namespace ionir {
    class Token {
    protected:
        TokenType type;

        std::string value;

        uint32_t startPosition;

        uint32_t lineNumber;

    public:
        static Token createDummy(uint32_t startPosition);

        Token(TokenType type, std::string value, uint32_t startPosition = 0, uint32_t lineNumber = 0);

        TokenType getType() const;

        std::string getValue() const;

        uint32_t getStartPosition() const;

        uint32_t getEndPosition() const;

        uint32_t getLineNumber() const;

        bool isDummy() const;

        bool operator==(const Token &other) const;

        bool operator!=(const Token &other) const;
    };

    std::ostream &operator<<(std::ostream &stream, Token &token);

    /**
     * An iterable list of tokens.
     */
    typedef Iterable<Token> TokenStream;
}
