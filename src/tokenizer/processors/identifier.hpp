#pragma once

#include "../processor.hpp"

namespace processor
{
    class Identifier : public Processor
    {
    public:
        using Processor::Processor;

        std::optional<Token> process(char character, state::Identifier *state)
        {
            if (buffer_begin() == '}')
            {
                auto flushed = flush_buffer(TokenType::PUNCTUATION);
                states.pop();
                append_buffer(character);
                return flushed;
            }

            if (state->substitution && character == '}')
            {
                auto flushed = flush(state, false);
                append_buffer(character);
                return flushed;
            }

            if (is_buffer_empty())
            {
                if (is::identifier::begin(character))
                {
                    return append_buffer(character);
                }
            }
            else
            {
                if (is::identifier::body(character))
                {
                    return append_buffer(character);
                }
            }

            if (is::whitespace(character))
            {
                return flush(state);
            }

            auto flushed = flush(state);
            append_buffer(character);
            return flushed;
        }

        std::optional<Token> flush(state::Identifier *state, bool pop_state = true)
        {
            if (is_buffer_empty())
            {
                return std::nullopt;
            }

            if (pop_state)
            {

                delete states.pop();
            }
            return flush_buffer(TokenType::IDENTIFIER);
        }
    };
}