#include "exceptions.hpp"
#include <boost/format.hpp>

MultipleDefinitions::MultipleDefinitions(std::string const &symbol_name)
  : std::runtime_error(boost::str(
        boost::format("symbol '%1%' has multiple definitions") % symbol_name)),
    m_symbol_name(symbol_name)
{
}

std::string
MultipleDefinitions::name() const
{
    return m_symbol_name;
}
