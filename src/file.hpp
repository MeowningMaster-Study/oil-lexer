#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "tokenizer/tokenizer.hpp"

namespace file
{
    std::string read(std::string file_path)
    {
        std::ifstream file_stream(file_path);
        std::stringstream buffer;
        buffer << file_stream.rdbuf();
        return buffer.str();
    }
};
