#include <iostream>
#include <stdexcept>

#include "file.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        throw std::runtime_error("Incorrect number of args");
    }
    std::string file_path = argv[1];

    auto file_content = file::read(file_path);
    auto tokenizer = new Tokenizer(file_content);
    while (true)
    {
        auto token = tokenizer->next();
        if (!token.has_value())
        {
            break;
        }
        std::cout << token.value() << '\n';
    }
}