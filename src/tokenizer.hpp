#pragma once

#include <string>
#include <vector>
#include <optional>

#include "token.hpp"
#include "specs/utils.hpp"
#include "state-stack.hpp"

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
    state::Stack states;

    inline bool is_buffer_empty()
    {
        return buffer.length() == 0;
    }

    std::optional<char> buffer_begin()
    {
        if (is_buffer_empty())
        {
            return std::nullopt;
        }
        return buffer[0];
    };

    Token flush_buffer(TokenType type)
    {
        Token token{type, buffer};
        buffer.clear();
        return token;
    }

    std::optional<Token> process_character(char character)
    {
        auto state = states.top();
        switch (state->type)
        {
        case state::Type::COMMAND:
            return process_command(character, (state::Command *)state);
        case state::Type::EXPRESSION:
            return process_expression(character);
        case state::Type::STRING:
            return process_string(character);
        case state::Type::COMMENT:
            return process_comment(character);

        default:
            throw std::runtime_error("Unknown state");
        }
    }

    std::optional<Token> append_buffer(char character)
    {
        buffer.push_back(character);
        return std::nullopt;
    }

    std::optional<Token> process_command(char character, state::Command *state)
    {
        if (character == ' ')
        {
            return flush_command_buffer(state);
        }

        if (character == '#')
        {
            states.push(new state::Comment());
            return flush_command_buffer(state);
        }

        if (!state->named)
        {
            if (is::command::name(character))
            {
                return append_buffer(character);
            }
        }
        else
        {
            if (is::command::argument(character))
            {
                return append_buffer(character);
            }
        }

        append_buffer(character);
        return flush_command_buffer(state);
    }

    std::optional<Token> flush_command_buffer(state::Command *state)
    {
        auto character_option = buffer_begin();
        if (!character_option.has_value())
        {
            return std::nullopt;
        }
        char character = character_option.value();

        if (!state->named)
        {
            if (is::command::name(character))
            {
                state->named = true;
                return flush_buffer(TokenType::IDENTIFIER);
            }
        }
        else
        {
            if (is::command::argument(character))
            {
                return flush_buffer(TokenType::LITERAL);
            }
        }

        return flush_buffer(TokenType::ILLEGAL);
    }

    std::optional<Token> process_expression(char character)
    {
    }

    std::optional<Token> process_string(char character)
    {
    }

    std::optional<Token> process_comment(char character)
    {
        if (character == '\n' || character == '\0')
        {
            states.pop();
        }
        return std::nullopt;
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
                if (is_buffer_empty())
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