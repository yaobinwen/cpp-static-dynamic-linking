#include "static_lib.hpp"

StaticLib::StaticLib(boost::filesystem::path const &p) : path(p) {}

StaticLib::StaticLib(
    boost::filesystem::path const &p,
    std::vector<ObjectFile> files)
  : path(p), obj_files(files)
{
}

bool
StaticLib::operator<(StaticLib const &rhs) const
{
    return this->path < rhs.path;
}
