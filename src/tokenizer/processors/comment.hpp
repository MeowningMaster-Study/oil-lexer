#pragma once

#include "../processor.hpp"

namespace processor
{
    class Comment : public Processor
    {
    public:
        using Processor::Processor;

        std::optional<Token> process(char character)
        {
            if (character == '\n')
            {
                delete states.pop();
                delete states.emplace(new state::Command());
            }
            return std::nullopt;
        }
    };
}