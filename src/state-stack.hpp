#pragma once

#include <vector>
#include <stdexcept>
#include <memory>

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
    int bracketsCount = 0;
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

using StatePtr = std::unique_ptr<State>;

class StateStack
{
private:
    std::vector<StatePtr> data;

public:
    StateStack()
    {
        data.push_back(std::make_unique<CommandState>());
    }

    void push(StatePtr state)
    {
        data.push_back(state);
    }

    StatePtr pop()
    {
        if (data.empty())
        {
            throw std::runtime_error("No state pop");
        }

        auto result = std::move(data.back());
        data.pop_back();
        return result;
    }

    State emplace(StatePtr state)
    {
        if (data.empty())
        {
            throw std::runtime_error("No state emplace");
        }

        data.emplace_back(state);
    }
};