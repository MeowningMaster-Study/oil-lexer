#pragma once

#include <string>
#include <algorithm>
#include <set>

#include "raw.hpp"

template <class T>
bool is_in_vector(std::vector<T> vector, T value)
{
    return std::find(vector.begin(), vector.end(), value) != vector.end();
}

bool is_punctuation(char character)
{
    return is_in_vector(punctuations_vector, character);
};

bool is_whitespace(char character)
{
    return is_in_vector(whitespace_vector, character);
};

bool is_number(char character)
{
    return character >= '0' && character <= '9';
};

bool is_letter(char character)
{
    return (character >= 'a' && character <= 'z') ||
           (character >= 'A' && character <= 'Z');
};

template <class T>
std::set<T> to_set(std::vector<T> vector)
{
    return std::set(vector.begin(), vector.end());
}