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

    char next_char()
    {
        return text[position++];
    }

    std::string buffer;
    state::Stack states;

    inline bool is_buffer_empty()
    {
        return buffer.length() == 0;
    }

    char buffer_begin()
    {
        return buffer[0];
    };

    char buffer_end()
    {
        return buffer[buffer.length() - 1];
    }

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
            return process_expression(character, (state::Expression *)state);
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
        // WHITESPACE
        if (character == ' ' || character == '\t' || character == '\0')
        {
            return flush_command_buffer(state);
        }

        if (character == '\n')
        {
            auto old_state = states.emplace(new state::Command());
            auto flushed = flush_command_buffer(state);
            delete old_state;
            return flushed;
        }

        // DENOTE
        if (state->denote)
        {
            append_buffer(character);
            state->denote = false;
            return std::nullopt;
        }

        if (character == '\\')
        {
            state->denote = true;
            return std::nullopt;
        }

        // COMMENT
        if (character == '#')
        {
            states.push(new state::Comment());
            return flush_command_buffer(state);
        }

        // SYMBOLS

        if (character == ';')
        {
            auto flushed = flush_command_buffer(state);
            append_buffer(character);
            return flushed;
        }

        char is_symbol = is::symbol(character);
        char begin_character = buffer_begin();
        bool begin_is_symbol = is::symbol(begin_character);
        if (is_symbol && (begin_character == '\0' || begin_is_symbol))
        {
            return append_buffer(character);
        }

        // COMMAND | KEYWORD | IDENTIFIER
        if (!state->named)
        {
            if (is::command::name(character))
            {
                return append_buffer(character);
            }
        }
        else if (is::command::argument(character))
        {
            return append_buffer(character);
        }

        // ILLEGAL
        auto flushed = flush_command_buffer(state);
        append_buffer(character);
        return flushed;
    }

    std::optional<Token> flush_command_buffer(state::Command *state)
    {
        auto begin_character = buffer_begin();
        if (begin_character == '\0')
        {
            return std::nullopt;
        }

        if (buffer == ";")
        {
            delete states.emplace(new state::Command());
            return flush_buffer(TokenType::PUNCTUATION);
        }

        if (!state->named)
        {
            if (is::command::name(begin_character))
            {
                state->named = true;
                return flush_buffer(TokenType::COMMAND);
            }
        }
        else
        {
            if (is::command::argument(begin_character))
            {
                return flush_buffer(TokenType::LITERAL);
            }
            if (begin_character == '$')
            {
                return flush_buffer(TokenType::IDENTIFIER);
            }
        }

        if (is::symbol(begin_character))
        {
            if (buffer == "_" || buffer_end() == '=')
            {
                delete states.emplace(new state::Expression());
            };
            return flush_buffer(TokenType::OPERATOR);
        }

        return flush_buffer(TokenType::ILLEGAL);
    }

    std::optional<Token> process_expression(char character, state::Expression *state)
    {
        // WHITESPACE
        if (character == ' ' || character == '\t' || character == '\0')
        {
            return flush_expression_buffer(state);
        }

        if (character == '\n')
        {
            auto old_state = states.emplace(new state::Command());
            auto flushed = flush_expression_buffer(state);
            delete old_state;
            return flushed;
        }

        // COMMENT
        if (character == '#')
        {
            states.push(new state::Comment());
            return flush_expression_buffer(state);
        }

        auto begin_character = buffer_begin();

        // IDENTIFIER
        if (
            (
                begin_character == '\0' &&
                is::expression::variable_start(character)) ||
            (is::expression::variable_start(begin_character) &&
             is::expression::variable_body(character)))
        {
            return append_buffer(character);
        }

        // NUMBER LITERAL
        if ((
                begin_character == '\0' &&
                is::expression::number_start(character)) ||
            (is::expression::number_start(begin_character) &&
             is::expression::number_body(character)))
        {
            return append_buffer(character);
        }

        // SYMBOL
        char is_symbol = is::symbol(character);
        bool begin_is_symbol = is::symbol(begin_character);
        if (is_symbol && (begin_character == '\0' || begin_is_symbol))
        {
            return append_buffer(character);
        }

        // ILLEGAL
        auto flushed = flush_expression_buffer(state);
        append_buffer(character);
        return flushed;
    }

    std::optional<Token> flush_expression_buffer(state::Expression *state)
    {
        auto begin_character = buffer_begin();
        if (begin_character == '\0')
        {
            return std::nullopt;
        }

        if (is::expression::variable_start(begin_character))
        {
            return flush_buffer(TokenType::IDENTIFIER);
        }

        if (is::expression::number_start(begin_character))
        {
            return flush_buffer(TokenType::LITERAL);
        }

        if (is::symbol(begin_character))
        {
            if (is::punctuation(begin_character))
            {
                return flush_buffer(TokenType::PUNCTUATION);
            }
            return flush_buffer(TokenType::OPERATOR);
        }

        return flush_buffer(TokenType::ILLEGAL);
    }

    std::optional<Token> process_string(char character)
    {
    }

    std::optional<Token> process_comment(char character)
    {
        if (character == '\n')
        {
            delete states.pop();
            delete states.emplace(new state::Command());
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
            if (position > text.length())
            {
                return std::nullopt;
            }

            auto character = this->next_char();
            next_token = process_character(character);
        } while (!next_token.has_value());
        return next_token;
    }

    // Tokens consume()
    // {
    //     Tokens tokens;
    //     while (true)
    //     {
    //         auto x = this->next();
    //         if (!x.has_value())
    //         {
    //             break;
    //         }
    //         tokens.push_back(x.value());
    //     }
    //     return tokens;
    // }
};