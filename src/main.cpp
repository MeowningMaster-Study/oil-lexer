#include <iostream>
#include <stdexcept>

#include "file.hpp"
#include "state-stack.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        throw std::runtime_error("Incorrect number of args");
    }
    std::string file_path = argv[1];

    auto tokens = tokenize_file(file_path);
    for (auto token : tokens)
    {
        std::cout << token << '\n';
    }
}