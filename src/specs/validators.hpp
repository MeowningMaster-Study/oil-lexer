#pragma once

#include <string>
#include <algorithm>
#include <set>

#include "dictionaries.hpp"

namespace is
{
    template <class T>
    bool in_vector(std::vector<T> vector, T value)
    {
        return std::find(vector.begin(), vector.end(), value) != vector.end();
    }

    bool punctuation(char character)
    {
        return in_vector(punctuations, character);
    };

    bool whitespace(char character)
    {
        return character == ' ' ||
               character == '\n' ||
               character == '\t';
    };

    bool number(char character)
    {
        return character >= '0' &&
               character <= '9';
    };

    bool letter(char character)
    {
        return (character >= 'a' && character <= 'z') ||
               (character >= 'A' && character <= 'Z');
    };

    bool symbol(char character)
    {
        return !whitespace(character) &&
               !number(character) &&
               !letter(character);
    }

    bool number_friend(char character)
    {
        return in_vector(number_friends, character);
    }

    bool keyword(std::string identifier)
    {
        return in_vector(keywords, identifier);
    }

    // template <class T>
    // std::set<T> to_set(std::vector<T> vector)
    // {
    //     return std::set(vector.begin(), vector.end());
    // }

    namespace command
    {
        bool name(char character)
        {
            return letter(character) ||
                   number(character) ||
                   character == '_';
        }

        bool argument(char character)
        {
            return name(character) ||
                   character == '.' ||
                   character == '-' ||
                   character == '/' ||
                   character == '*' ||
                   character == '?';
        }
    }

    namespace expression
    {
        bool variable_start(char character)
        {
            return letter(character) ||
                   character == '_';
        }

        bool variable_body(char character)
        {
            return variable_start(character) ||
                   number(character) ||
                   character == '.';
        }

        bool number_start(char character)
        {
            return number(character);
        }

        bool number_body(char character)
        {
            return number(character) || number_friend(character);
        }
    }

    namespace identifier
    {
        bool begin(char character)
        {
            return letter(character) ||
                   character == '_';
        }

        bool body(char character)
        {
            return begin(character) ||
                   number(character);
        }
    }
}
