#pragma once

#include <vector>
#include <string>

// source https://github.com/oilshell/oil/blob/master/doc/oil-help-topics.md

std::vector<std::string> keywords_vector = {
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

std::vector<char> punctuations_vector = {
    '(',
    ')',
    '{',
    '}',
    '[',
    ']',
    ',',
    ';',
};

std::vector<char> whitespace_vector = {
    ' ',
    '\n',
    '\t',
    '\0'};

std::vector<std::string> symbol_operators_vector = {
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

std::vector<std::string> word_operators_vector = {
    "is",
    "in",
    "not",
    "and",
    "or",
};
