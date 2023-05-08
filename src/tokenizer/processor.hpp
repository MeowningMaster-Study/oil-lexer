#pragma once

#include <optional>
#include <string>

#include "../token.hpp"
#include "../state-stack.hpp"

namespace processor
{
    class Processor
    {
    public:
        state::Stack &states;
        std::string &buffer;

        explicit Processor(state::Stack &states, std::string &buffer) : states(states), buffer(buffer) {}

        virtual std::optional<Token> process(char character, state::State *state) {}

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

        std::optional<Token> append_buffer(char character)
        {
            buffer.push_back(character);
            return std::nullopt;
        }

        Token flush_buffer(TokenType type)
        {
            Token token{type, buffer};
            buffer.clear();
            return token;
        }

        bool flush_string_begin()
        {
            if (buffer == "\"")
            {
                states.push(new state::String(state::StringType::DOUBLE_QUOTED, false));
                buffer.clear();
                return true;
            }

            if (buffer == "\'")
            {
                states.push(new state::String(state::StringType::SINGLE_QUOTED, false));
                buffer.clear();
                return true;
            }

            if (buffer == "$'")
            {
                states.push(new state::String(state::StringType::C_STYLE, false));
                buffer.clear();
                return true;
            }

            return false;
        }
    };
}