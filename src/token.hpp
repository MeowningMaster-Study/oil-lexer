#pragma once

#include <string>
#include <vector>

enum TokenType
{
    COMMAND,
    IDENTIFIER,
    KEYWORD,
    OPERATOR,
    LITERAL,
    PUNCTUATION,
    ILLEGAL
};

class TokenTypeUtils
{
public:
    static std::string toString(TokenType type)
    {
        switch (type)
        {
        case TokenType::COMMAND:
            return "Command";
        case TokenType::IDENTIFIER:
            return "Identifier";
        case TokenType::KEYWORD:
            return "Keyword";
        case TokenType::OPERATOR:
            return "Operator";
        case TokenType::LITERAL:
            return "Literal";
        case TokenType::PUNCTUATION:
            return "Punctuation";
        case TokenType::ILLEGAL:
            return "Illegal";
        default:
            throw std::runtime_error("Unknown token type");
        }
    }
};

class Token
{
public:
    TokenType type;
    std::string value;

    friend std::ostream &operator<<(std::ostream &stream, const Token &token)
    {
        auto tokenType = TokenTypeUtils::toString(token.type);
        return stream << tokenType << "(\"" << token.value << "\")";
    }
};

using Tokens = std::vector<Token>;