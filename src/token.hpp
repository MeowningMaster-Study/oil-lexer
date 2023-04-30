#include <string>

enum TokenType
{
    IDENTIFIER,
    KEYWORD,
    OPERATOR,
    LITERAL,
    PUNCTUATION,
};

class Token
{
    TokenType type;
    std::string value;
};