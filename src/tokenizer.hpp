#include <iterator>

class Tokenizer
{
    struct Iterator
    {
        using iterator_category = std::input_iterator_tag;
        using difference_type = int;
        // using value_type = Token;
    };
};