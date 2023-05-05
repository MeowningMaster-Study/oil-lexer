#pragma once

#include <string>
#include <vector>
#include <optional>

#include "token.hpp"
#include "specs/utils.hpp"

enum CommentState
{
    NONE,
    SINGLE_LINE,
    MULTILINE
};

class Tokenizer
{
private:
    int position = 0;
    std::string text;

    std::optional<char> next_char()
    {
        if (position >= text.length())
        {
            return std::nullopt;
        }
        return text[position++];
    }

    std::string buffer;
    StateStack states;

    /**
     * Is similar to identifier. May be:
     * - identifier
     * - keyword
     * - wordly operator
     */
    bool is_identifiery = false;

    /**
     * Is similar to number literal (integer or float)
     */
    bool is_numbery = false;

    std::optional<Token> process_character(char character)
    {
        auto state = states.top();
        switch (state->type)
        {
        case StateType::COMMAND:
            return process_command(character);
        case StateType::EXPRESSION:
            return process_expression(character);
        case StateType::STRING:
            return process_string(character);
        case StateType::COMMENT:
            return process_comment(character);

        default:
            throw std::runtime_error("Unknown state");
        }

        // if (is_punctuation(character))
        // {
        //     return Token{
        //         TokenType::PUNCTUATION, std::string{character}};
        // }

        // return Token{TokenType::ILLEGAL, std::string{character}};
    }

    std::optional<Token> process_command(char character)
    {
    }

    std::optional<Token> process_expression(char character)
    {
    }

    std::optional<Token> process_string(char character)
    {
    }

    std::optional<Token> process_comment(char character)
    {
    }

public:
    Tokenizer(std::string text) : text(text) {}

    std::optional<Token> next()
    {
        std::optional<Token> next_token;
        do
        {
            auto optional_char = this->next_char();
            if (!optional_char.has_value())
            {
                if (buffer.length() == 0)
                {
                    return std::nullopt;
                }

                // TODO: flush buffer
            }
            char character = optional_char.value();
            next_token = process_character(character);
        } while (!next_token.has_value());
        return next_token;
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