#pragma once

#include "symbol.hpp"
#include <boost/filesystem.hpp>
#include <set>

struct ObjectFile
{
    /// Path of the file.
    boost::filesystem::path path;

    /// Symbols that this object file exports.
    std::set<Symbol> exported_symbols;

    /// Symbols that this object file imports (i.e., requests).
    std::set<Symbol> imported_symbols;

    /// Symbols that this object file uses internally.
    std::set<Symbol> internal_symbols;

    /// Constructor
    ObjectFile(boost::filesystem::path const &p);

    /// Constructor
    ObjectFile(
        boost::filesystem::path const &p,
        std::set<Symbol> const &exported,
        std::set<Symbol> const &imported,
        std::set<Symbol> const &internal = std::set<Symbol>());

    /// Copy constructor
    ObjectFile(ObjectFile const &) = default;

    /// Copy assignment operator
    ObjectFile &
    operator=(ObjectFile const &) = default;

    /// Move constructor
    ObjectFile(ObjectFile &&) = default;

    /// Move assignment operator
    ObjectFile &
    operator=(ObjectFile &&) = default;

    /// Destructor
    ~ObjectFile() = default;

    /// Less-than operator
    bool
    operator<(ObjectFile const &rhs) const;
};
