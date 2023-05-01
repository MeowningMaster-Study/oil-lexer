#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "tokenizer.hpp"

std::string read_file(std::string file_path)
{
    std::ifstream file_stream(file_path);
    std::stringstream buffer;
    buffer << file_stream.rdbuf();
    return buffer.str();
}

Tokens tokenize_file(std::string file_path)
{
    auto file_content = read_file(file_path);
    auto tokenizer = new Tokenizer(file_content);
    return tokenizer->consume();
}