#pragma once

#include <string>

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
