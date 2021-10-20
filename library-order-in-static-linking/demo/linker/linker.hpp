#pragma once

#include "object_file.hpp"
#include "static_lib.hpp"
#include "symbol.hpp"

#include <set>

/// The linker that demonstrates the linking algorithm.
class Linker
{
public:
    /// Constructor.
    Linker();

    /// Destructor.
    ~Linker();

    /// Link an object file.
    void
    link(ObjectFile const &obj_file);

    /// Link a static library.
    void
    link(StaticLib const &static_lib);

    /// Report undefined symbols.
    std::set<Symbol>
    report_undefined() const;

private:
    /// Whether the given object file exports any symbols that are being
    /// requested by others.
    bool
    exports_undefined(ObjectFile const &f) const;

    std::set<Symbol> m_exported;
    std::set<Symbol> m_undefined;
};
