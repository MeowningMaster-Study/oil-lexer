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
                return flush_string(state);
            }

            if (character == '\n')
            {
                if (state->multiline)
                {
                    append_buffer(character);
                }
                else
                {
                    auto flushed = flush_string(state);
                    delete states.emplace(new state::Command());
                    return flushed;
                }
            }

            switch (type)
            {
            case state::StringType::DOUBLE_QUOTED:
            case state::StringType::C_STYLE:
                // DENOTE
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
                if (character != '\'')
                {
                    return append_buffer(character);
                }
                break;
            case state::StringType::DOUBLE_QUOTED:
                if (character == '"')
                {
                    return flush_string(state);
                }
                return append_buffer(character);
                break;

            default:
                throw std::runtime_error("Unknown string type");
            }
        }

        std::optional<Token> flush_string(state::String *state)
        {
            auto begin_character = buffer_begin();
            if (begin_character == '\0')
            {
                return std::nullopt;
            }

            states.pop();
            return flush_buffer(TokenType::LITERAL);
        }
    };
}