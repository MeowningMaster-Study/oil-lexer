#pragma once

#include <vector>
#include <stdexcept>

// https://www.oilshell.org/release/latest/doc/command-vs-expression-mode.html

enum StateType
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
    StateType type;
};

struct CommandState : State
{
    /**
     * - echo /$
     */
    bool denote = false;

    /**
     * - echo $(hostname)
     */
    bool substitution = false;
};

struct ExpressionState : State
{
    /**
     * > echo $[2 + 2]
     */
    bool substitution = false;

    /**
     * Disambiguates such expressions
     * > echo $[42 + a[i]]
     */
    int brackets_count = 0;

    /**
     * Indicates wheter expression name is already tokenized
     */
    bool named = false;
};

enum StringStateType
{
    /** Allow interpolation with `$` */
    DOUBLE_QUOTED,
    /** All characters are literal */
    SINGLE_QUOTED,
    /** Respect backslash character escapes */
    C_STYLE
};

struct StringState : State
{
public:
    StringStateType type;
    bool multiline = false;
    /**
     * > echo "\$ \" \\ "
     */
    bool denote = false;
};

using StatePtr = State *;

class StateStack
{
private:
    std::vector<StatePtr> data;

public:
    StateStack()
    {
        data.push_back(new CommandState);
    }

    void push(StatePtr state)
    {
        data.push_back(state);
    }

    StatePtr top()
    {
        if (data.empty())
        {
            throw std::runtime_error("No state on top");
        }

        return data.back();
    }

    StatePtr pop()
    {
        auto result = this->top();
        data.pop_back();
        return result;
    }

    void emplace(StatePtr state)
    {
        if (data.empty())
        {
            throw std::runtime_error("No state to emplace");
        }

        data.emplace_back(state);
    }
};