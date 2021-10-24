#include "symbol.hpp"
#include <algorithm>

Symbol::Symbol(std::string const &n) : name(n)
{
    // Empty
}

bool
Symbol::operator<(Symbol const &rhs) const
{
    return this->name < rhs.name;
}

std::vector<std::string>
symbol_set_to_string_vector(std::set<Symbol> const &symbol_set)
{
    std::vector<std::string> v;
    std::transform(
        symbol_set.begin(),
        symbol_set.end(),
        std::back_inserter(v),
        [](Symbol const &s) { return s.name; });
    return v;
}
