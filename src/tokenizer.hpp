#include <string>
#include <vector>
#include <optional>

#include "token.hpp"

class Tokenizer
{
public:
    int position = 0;
    std::string text;

    Tokenizer(std::string text) : text(text) {}

    std::optional<Token> next()
    {
        return std::nullopt;
    }

    Tokens consume()
    {
        Tokens tokens;
        while (true)
        {
            auto x = this->next();
            if (!x.has_value())
            {
                break;
            }
            tokens.push_back(x.value());
        }
        return tokens;
    }
};