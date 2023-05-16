#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>

#include "./state.hpp"

namespace state
{
    class Stack
    {
    private:
        std::vector<Ptr> data;
        bool log = false;

    public:
        Stack()
        {
            this->push(new Command());
        }

        void push(Ptr state)
        {
            data.push_back(state);
            if (this->log)
            {
                std::cout << "> State pushed: " << state::TypeUtils::toString(state->type) << "\n";
            }
        }

        Ptr top()
        {
            if (data.empty())
            {
                throw std::runtime_error("No state on top");
            }

            return data.back();
        }

        Ptr pop()
        {

            auto result = this->top();
            data.pop_back();
            if (this->log)
            {
                std::cout << "> State popped. New state is: " << state::TypeUtils::toString(this->top()->type) << "\n";
            }
            return result;
        }

        Ptr emplace(Ptr state)
        {
            if (data.empty())
            {
                throw std::runtime_error("No state to emplace");
            }

            auto top_state = top();
            data.emplace_back(state);
            if (this->log)
            {
                std::cout << "> State emplaced. New state is: " << state::TypeUtils::toString(state->type) << "\n";
            }
            return top_state;
        }
    };
}
