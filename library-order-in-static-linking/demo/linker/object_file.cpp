#include "object_file.hpp"

ObjectFile::ObjectFile(boost::filesystem::path const &p) : path(p) {}

ObjectFile::ObjectFile(
    boost::filesystem::path const &p,
    std::set<Symbol> const &exported,
    std::set<Symbol> const &imported,
    std::set<Symbol> const &internal)
  : path(p),
    exported_symbols(exported),
    imported_symbols(imported),
    internal_symbols(internal)
{
}

bool
ObjectFile::operator<(ObjectFile const &rhs) const
{
    return this->path < rhs.path;
}
