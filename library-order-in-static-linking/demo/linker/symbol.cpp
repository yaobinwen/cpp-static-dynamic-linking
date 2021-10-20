#include "symbol.hpp"

Symbol::Symbol(std::string const &n) : name(n)
{
    // Empty
}

bool
Symbol::operator<(Symbol const &rhs) const
{
    return this->name < rhs.name;
}
