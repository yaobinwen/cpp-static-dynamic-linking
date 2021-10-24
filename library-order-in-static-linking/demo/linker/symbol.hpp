#pragma once

#include <set>
#include <string>
#include <vector>

/// A symbol that is exported or requested by the object files.
struct Symbol
{
    /// Name of the symbol.
    std::string name;

    /// Constructor
    Symbol(std::string const &n);

    /// Copy constructor
    Symbol(Symbol const &) = default;

    /// Copy assignment operator
    Symbol &
    operator=(Symbol const &) = default;

    /// Move constructor.
    Symbol(Symbol &&) = default;

    /// Move assignment operator
    Symbol &
    operator=(Symbol &&) = default;

    /// Destructor
    ~Symbol() = default;

    /// Less-than operator.
    bool
    operator<(Symbol const &rhs) const;
};

/// Helper function: transform a std::set<Symbol> to std::vector<std::string>.
std::vector<std::string>
symbol_set_to_string_vector(std::set<Symbol> const &symbol_set);
