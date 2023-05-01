#pragma once

#include <vector>
#include <string>

// source https://github.com/oilshell/oil/blob/master/doc/oil-help-topics.md

std::vector<std::string> keywords = {
    // Command Language
    "proc",
    "if"
    "case",
    "while",
    "for",
    // Expression Language and Assignments
    "const",
    "var",
    "setvar",
    "setglobal",
    "setref",
};

std::vector<char> punctuations = {
    '(',
    ')',
    '{',
    '}',
    '[',
    ']',
    ',',
    ';',
};

std::vector<char> whitespaces = {
    ' ',
    '\n',
    '\t',
    '\0'};

std::vector<std::string> symbol_operators = {
    "++",
    "===",
    "!==",
    "~==",
    "<",
    "<=",
    ">",
    ">=",
    "+",
    "-",
    "*",
    "/",
    "//",
    "%",
    "**",
    "~",
    "&",
    "|",
    "^",
    "<<",
    ">>",
    "!~",
    "~~",
    "!~~",
    "$",
    "@",
    "_",
    ":-"};

/**
 * Operators similar to identifiers
 */
std::vector<std::string> wordly_operators = {
    "is",
    "in",
    "not",
    "and",
    "or",
};

/**
 * Characters that could be found inside integer or float literals
 * @link https://github.com/oilshell/oil/blob/master/doc/oil-help-topics.md#--expression-language-and-assignments-expr-lang
 */
std::vector<char> number_friends = {
    '_',
    'x',
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'o',
    'b',
    '.'};