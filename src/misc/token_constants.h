#pragma once

#include <vector>
#include <map>
#include <string>
#include <regex>
#include "syntax/token_type.h"
#include "misc/regex.h"

namespace ionir
{
class TokenConstants
{
private:
	std::map<std::string, TokenType> simple;

	std::vector<std::pair<std::regex, TokenType>> complex;

	void pushComplex(std::regex regex, TokenType tokenType)
	{
		this->complex.push_back(std::make_pair(regex, tokenType));
	}

public:
	TokenConstants()
	{
		// Register symbols.
		this->simple["@"] = TokenType::SymbolAt;
		this->simple[":"] = TokenType::SymbolColon;
		this->simple["$"] = TokenType::SymbolDollar;
		this->simple["#"] = TokenType::SymbolHash;
		this->simple["("] = TokenType::SymbolParenthesesL;
		this->simple[")"] = TokenType::SymbolParenthesesR;
		this->simple["["] = TokenType::SymbolBracketL;
		this->simple["]"] = TokenType::SymbolBracketR;
		this->simple[","] = TokenType::SymbolComma;
		this->simple["~"] = TokenType::SymbolTilde;
		this->simple["="] = TokenType::SymbolEqual;
		this->simple["%"] = TokenType::SymbolPercent;
		this->simple[";"] = TokenType::SymbolSemiColon;

		// Register instructions.
		this->simple["create"] = TokenType::InstCreate;
		this->simple["call"] = TokenType::InstCall;
		this->simple["set"] = TokenType::InstSet;
		this->simple["end"] = TokenType::InstEnd;

		// Register keywords.
		this->simple["func"] = TokenType::KeywordFunction;

		// Initialize complex map.
		this->pushComplex(Regex::identifier, TokenType::Identifier);
		this->pushComplex(Regex::string, TokenType::LiteralString);
		this->pushComplex(Regex::decimal, TokenType::LiteralDecimal);
		this->pushComplex(Regex::integer, TokenType::LiteralInteger);
		this->pushComplex(Regex::character, TokenType::LiteralCharacter);
		this->pushComplex(Regex::whitespace, TokenType::Whitespace);
	}

	std::map<std::string, TokenType> getSimpleIdentifiers() const
	{
		return this->simple;
	}

	std::vector<std::pair<std::regex, TokenType>> getComplexIdentifiers() const
	{
		return this->complex;
	}
};
} // namespace ionir
