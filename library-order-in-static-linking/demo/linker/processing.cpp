#include "processing.hpp"
#include <boost/algorithm/string/join.hpp>
#include <sstream>

std::string
ObjectFileProcessing::str() const
{
    std::ostringstream oss;

    oss << "Processing"
        << "<";

    oss << "path: [" << path << "]";
    oss << "; ";
    oss << "action: [" << ActionNames[static_cast<size_t>(action)] << "]";
    oss << "; ";
    oss << "exports: ["
        << boost::algorithm::join(symbol_set_to_string_vector(exports), ",")
        << "]";
    oss << "; ";
    oss << "requests: ["
        << boost::algorithm::join(symbol_set_to_string_vector(requests), ",")
        << "]";
    oss << "; ";
    oss << "resolves: ["
        << boost::algorithm::join(symbol_set_to_string_vector(resolves), ",")
        << "]";
    oss << ">";

    return oss.str();
}
