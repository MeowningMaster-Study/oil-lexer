#pragma once

#include "../processor.hpp"

namespace processor
{
    class Command : public Processor
    {
    public:
        using Processor::Processor;

        std::optional<Token> process(char character, state::Command *state)
        {
            // WHITESPACE
            if (character == ' ' || character == '\t' || character == '\0')
            {
                return flush_command(state);
            }

            if (character == '\n')
            {
                auto old_state = states.emplace(new state::Command());
                auto flushed = flush_command(state);
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
                return flush_command(state);
            }

            char begin_character = buffer_begin();

            // COMMAND | KEYWORD | IDENTIFIER
            if (!state->named)
            {
                if (!is::symbol(begin_character) && is::command::name(character))
                {
                    return append_buffer(character);
                }
            }
            else if (
                (begin_character == '\0' && character == '$') ||
                ((begin_character == '$' || is::command::argument(begin_character)) && is::command::argument(character)))
            {
                return append_buffer(character);
            }

            // SYMBOLS

            if (character == ';')
            {
                auto flushed = flush_command(state);
                append_buffer(character);
                return flushed;
            }

            char is_symbol = is::symbol(character);
            bool begin_is_symbol = is::symbol(begin_character);
            if (is_symbol)
            {
                if (begin_character == '\0')
                {
                    return append_buffer(character);
                }
                if (begin_is_symbol)
                {
                    auto concated = buffer + character;
                    // EXPRESSION SUBSTITUTION
                    if (
                        (concated == "$["))
                    {
                        state::Expression *new_state = new state::Expression();
                        new_state->substitution = true;
                        states.push(new_state);
                        append_buffer(character);
                        return flush_buffer(TokenType::PUNCTUATION);
                    }

                    if (
                        (concated == "||") ||
                        (concated == "&&"))
                    {
                        delete states.emplace(new state::Command());
                        append_buffer(character);
                        return flush_buffer(TokenType::OPERATOR);
                    }
                }
            }

            // ILLEGAL
            auto flushed = flush_command(state);
            append_buffer(character);
            return flushed;
        }

        std::optional<Token> flush_command(state::Command *state)
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

            if (buffer == "_" || buffer_end() == '=')
            {
                delete states.emplace(new state::Expression());
                return flush_buffer(TokenType::OPERATOR);
            }

            if (flush_string_begin())
            {
                return std::nullopt;
            }

            if (
                is::symbol(begin_character) &&
                !is::command::argument(begin_character) &&
                begin_character != '$')
            {
                return flush_buffer(TokenType::OPERATOR);
            }

            if (!state->named)
            {
                if (is::command::name(begin_character))
                {
                    state->named = true;
                    if (is::keyword(buffer))
                    {
                        return flush_buffer(TokenType::KEYWORD);
                    }
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

            return flush_buffer(TokenType::ILLEGAL);
        }
    };
}