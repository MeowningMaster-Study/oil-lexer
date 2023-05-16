#pragma once

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
        STRING,
        IDENTIFIER,
    };

    class TypeUtils
    {
    public:
        static std::string toString(Type type)
        {
            switch (type)
            {
            case Type::COMMAND:
                return "Command";
            case Type::EXPRESSION:
                return "Expression";
            case Type::COMMENT:
                return "Comment";
            case Type::STRING:
                return "String";
            case Type::IDENTIFIER:
                return "Identifier";
            default:
                throw std::runtime_error("Unknown type");
            }
        }
    };

    struct State
    {
        Type type;

        State(Type type) : type(type){};
    };

    using Ptr = State *;

    struct Command : State
    {
        Command() : State(Type::COMMAND){};

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
        Expression(bool substitution = false) : State(Type::EXPRESSION), substitution(substitution){};

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
        String(
            StringType string_type,
            bool multiline) : State(Type::STRING),
                              string_type(string_type),
                              multiline(multiline){};

        StringType string_type;
        bool multiline;
        /**
         * > echo "\$ \" \\ "
         */
        bool denote = false;
    };

    struct Comment : State
    {
        Comment() : State(Type::COMMENT){};
    };

    struct Identifier : State
    {
        Identifier(bool substitution) : State(Type::IDENTIFIER), substitution(substitution){};

        bool substitution;
    };
}
