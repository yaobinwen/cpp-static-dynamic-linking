#include "simplestr.hpp"
#include <cctype>

namespace simplestr
{

bool is_all_alphanumeric(std::string const &s)
{
    for (auto const &ch: s)
    {
        if (std::isalnum(static_cast<unsigned char>(ch)) == 0)
        {
            return false;
        }
    }

    return true;
}

}   // namespace simplestr
