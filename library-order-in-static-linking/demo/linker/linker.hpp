#pragma once

#include "object_file.hpp"
#include "processing.hpp"
#include "static_lib.hpp"
#include "symbol.hpp"
#include <set>
#include <string>
#include <vector>

/// The linker that demonstrates the linking algorithm.
class Linker
{
public:
    /// Constructor.
    Linker();

    /// Destructor.
    ~Linker();

    /// Link an object file.
    ObjectFileProcessing
    link(ObjectFile const &obj_file);

    /// Link a static library.
    std::vector<ObjectFileProcessing>
    link(StaticLib const &static_lib);

    /// Return the current states as a string for representation.
    std::string
    str() const;

    /// Return all the processing steps.
    std::vector<ObjectFileProcessing>
    get_all_processing_steps() const;

    /// Return the last processing step.
    ObjectFileProcessing
    get_last_processing_step() const;

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

    std::vector<ObjectFileProcessing> m_processing;
};
