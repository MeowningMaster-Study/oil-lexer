#include <iostream>

int main(int argc, char const *argv[])
{
    if (argc != 1)
    {
        throw std::runtime_error("Incorrect number of args");
    }

    std::string file_path = argv[0];
    std::cout << file_path;
}