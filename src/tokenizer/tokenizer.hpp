#pragma once

#include <string>
#include <vector>
#include <optional>

#include "../token.hpp"
#include "../specs/validators.hpp"
#include "../state/index.hpp"

#include "processors/index.hpp"

class Tokenizer
{
private:
    int position = 0;
    std::string text;

    char next_char()
    {
        return text[position++];
    }

    state::Stack states;
    std::string buffer;

    processor::Command command_processor;
    processor::Expression expression_processor;
    processor::String string_processor;
    processor::Comment comment_processor;
    processor::Identifier identifier_processor;

    std::optional<Token>
    process_character(char character)
    {
        auto state = states.top();
        switch (state->type)
        {
        case state::Type::COMMAND:
            return command_processor.process(character, (state::Command *)state);
        case state::Type::EXPRESSION:
            return expression_processor.process(character, (state::Expression *)state);
        case state::Type::STRING:
            return string_processor.process(character, (state::String *)state);
        case state::Type::COMMENT:
            return comment_processor.process(character);
        case state::Type::IDENTIFIER:
            return identifier_processor.process(character, (state::Identifier *)state);

        default:
            throw std::runtime_error("Unknown state");
        }
    }

public:
    Tokenizer(std::string text) : text(text),
                                  command_processor(states, buffer),
                                  expression_processor(states, buffer),
                                  string_processor(states, buffer),
                                  comment_processor(states, buffer),
                                  identifier_processor(states, buffer) {}

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
};