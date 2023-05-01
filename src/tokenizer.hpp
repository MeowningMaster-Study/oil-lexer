#pragma once

#include <string>
#include <vector>
#include <optional>

#include "token.hpp"
#include "specs/utils.hpp"

class Tokenizer
{
private:
    int position = 0;
    std::string text;

    std::optional<char> next_char()
    {
        if (position > text.length())
        {
            return std::nullopt;
        }
        return text[position++];
    }

    std::string buffer;

public:
    Tokenizer(std::string text) : text(text) {}

    std::optional<Token> break_buffer() {}

    std::optional<Token> next()
    {
        auto optional_char = this->next_char();
        if (!optional_char.has_value() && buffer.length() == 0)
        {
            return std::nullopt;
        }
        char character = optional_char.value();

        if (is_punctuation(character))
        {
            return Token{
                TokenType::PUNCTUATION, std::string{character}};
        }

        return Token{TokenType::ILLEGAL, std::string{character}};
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