#pragma once

#include "symbol.hpp"
#include <boost/filesystem.hpp>
#include <set>
#include <string>

enum Action
{
    UNKNOWN = 0,
    PROCESSED,
    SKIPPED_SCANNED,
    SKIPPED_NO_EXPORTING_UNDEFINED,
    COUNT
};

const char * const ActionNames [Action::COUNT] = {
    "Unknown",
    "Processed",
    "Skipped (Scanned)",
    "Skipped (No exporting undefined)"
};

struct ObjectFileProcessing
{
    boost::filesystem::path path;

    bool is_obj_file;

    Action action;

    std::set<Symbol> exports;
    std::set<Symbol> requests;
    std::set<Symbol> resolves;

    std::string
    str() const;
};
