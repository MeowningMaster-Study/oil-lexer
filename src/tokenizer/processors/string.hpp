#pragma once

#include "../processor.hpp"

namespace processor
{
    class String : public Processor
    {
    public:
        using Processor::Processor;

        std::optional<Token> process(char character, state::String *state)
        {
            auto type = state->string_type;

            if (character == '\0')
            {
                return flush(state);
            }

            if (character == '\n')
            {
                if (state->multiline)
                {
                    append_buffer(character);
                }
                else
                {
                    auto flushed = flush(state);
                    delete states.emplace(new state::Command());
                    return flushed;
                }
            }

            // DENOTE
            switch (type)
            {
            case state::StringType::DOUBLE_QUOTED:
            case state::StringType::C_STYLE:

                if (state->denote)
                {
                    state->denote = false;
                    return append_buffer(character);
                }
                if (character == '\\')
                {
                    state->denote = true;
                    return std::nullopt;
                }
                break;
            }

            switch (type)
            {
            case state::StringType::SINGLE_QUOTED:
            case state::StringType::C_STYLE:
                if (character == '\'')
                {
                    return flush(state);
                }
                return append_buffer(character);
                break;
            case state::StringType::DOUBLE_QUOTED:
                if (character == '"')
                {
                    return flush(state);
                }
                if (character == '$')
                {
                    auto flushed = flush(state, false);
                    append_buffer(character);
                    return flushed;
                }
                if (buffer_begin() == '$')
                {
                    return process_substitution(character);
                }
                return append_buffer(character);
                break;

            default:
                throw std::runtime_error("Unknown string type");
            }

            throw std::runtime_error("Unreachable state");
        }

        std::optional<Token> flush(state::String *state, bool pop_state = true)
        {
            if (pop_state)
            {
                states.pop();
            }
            return flush_buffer(TokenType::LITERAL);
        }

        std::optional<Token> process_substitution(char character)
        {
            // EXPRESSION SUBSTITUTION
            if (character == '[')
            {
                auto *new_state = new state::Expression();
                new_state->substitution = true;
                states.push(new_state);
                append_buffer(character);
                return flush_buffer(TokenType::PUNCTUATION);
            }
            // COMMAND SUBSTITUTION
            if (character == '(')
            {
                auto *new_state = new state::Command();
                new_state->substitution = true;
                states.push(new_state);
                append_buffer(character);
                return flush_buffer(TokenType::PUNCTUATION);
            }
            // VARIABLE SUBSTITUTION
            if (character == '{')
            {
                auto *new_state = new state::Identifier(true);
                states.push(new_state);
                append_buffer(character);
                return flush_buffer(TokenType::PUNCTUATION);
            }

            buffer.clear();
            auto *new_state = new state::Identifier(false);
            states.push(new_state);
            append_buffer(character);
            return Token(TokenType::PUNCTUATION, "$");
        }
    };
}