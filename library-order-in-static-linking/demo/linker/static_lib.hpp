#pragma once

#include "object_file.hpp"
#include <boost/filesystem.hpp>
#include <vector>

/// A static library.
struct StaticLib
{
    /// Path of the library.
    boost::filesystem::path path;

    /// The object files that this static library contains.
    std::vector<ObjectFile> obj_files;

    /// Constructor
    StaticLib(boost::filesystem::path const &p);

    /// Constructor
    StaticLib(boost::filesystem::path const &p, std::vector<ObjectFile> files);

    /// Copy constructor
    StaticLib(StaticLib const &) = default;

    /// Copy assignment operator
    StaticLib &
    operator=(StaticLib const &) = default;

    /// Move constructor
    StaticLib(StaticLib &&) = default;

    /// Move assignment operator
    StaticLib &
    operator=(StaticLib &&) = default;

    /// Destructor
    ~StaticLib() = default;

    /// Less-than operator
    bool
    operator<(StaticLib const &rhs) const;
};
