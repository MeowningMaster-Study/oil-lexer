#pragma once

#include <vector>
#include <stdexcept>

// https://www.oilshell.org/release/latest/doc/command-vs-expression-mode.html

namespace state
{
    enum Type
    {
        /**
         * Shell command
         * > git config --system user.name "MeowningMaster"
         * > cd ~
         */
        COMMAND,
        /**
         * Expression
         * > var x = 42 + f(x)
         * > = 2 + 2
         * > _ L.append(x)
         * > echo $[2 + 2]
         * > echo "2 + 2 = $[2 + 2]"
         */
        EXPRESSION,
        /**
         * Comment
         * > echo "hi" # prints hi
         */
        COMMENT,
        STRING
    };

    struct State
    {
        Type type;
    };

    struct Command : State
    {
        Command() { this->type = Type::COMMAND; };

        /**
         * - echo \$
         */
        bool denote = false;

        /**
         * - echo $(hostname)
         */
        bool substitution = false;

        /**
         * Indicates wheter expression name is already tokenized
         */
        bool named = false;
    };

    struct Expression : State
    {
        Expression() { this->type = Type::EXPRESSION; };

        /**
         * > echo $[2 + 2]
         */
        bool substitution = false;

        /**
         * Disambiguates such expressions
         * > echo $[42 + a[i]]
         */
        int brackets_count = 0;
    };

    enum StringType
    {
        /** Allow interpolation with `$` */
        DOUBLE_QUOTED,
        /** All characters are literal */
        SINGLE_QUOTED,
        /** Respect backslash character escapes */
        C_STYLE
    };

    struct String : State
    {
        String() { this->type = Type::STRING; };

        StringType string_type;
        bool multiline = false;
        /**
         * > echo "\$ \" \\ "
         */
        bool denote = false;
    };

    struct Comment : State
    {
        Comment() { this->type = Type::COMMENT; };
    };

    using Ptr = State *;

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
