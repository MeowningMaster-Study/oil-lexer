#pragma once

#include <vector>
#include <stdexcept>

#include "./state.hpp"

namespace state
{
    class Stack
    {
    private:
        std::vector<Ptr> data;

    public:
        Stack()
        {
            data.push_back(new Command());
        }

        void push(Ptr state)
        {
            data.push_back(state);
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
            return top_state;
        }
    };
}
