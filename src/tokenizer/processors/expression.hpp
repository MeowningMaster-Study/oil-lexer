#pragma once

#include "../processor.hpp"

namespace processor
{
    class Expression : public Processor
    {
    public:
        using Processor::Processor;

        std::optional<Token> process(char character, state::Expression *state)
        {
            // WHITESPACE
            if (character == ' ' || character == '\t' || character == '\0')
            {
                return flush_expression(state);
            }

            if (character == '\n')
            {
                auto old_state = states.emplace(new state::Command());
                auto flushed = flush_expression(state);
                delete old_state;
                return flushed;
            }

            // COMMENT
            if (character == '#')
            {
                states.push(new state::Comment());
                return flush_expression(state);
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
            auto flushed = flush_expression(state);
            append_buffer(character);
            return flushed;
        }

        std::optional<Token> flush_expression(state::Expression *state)
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

            if (flush_string_begin())
            {
                return std::nullopt;
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
    };
}