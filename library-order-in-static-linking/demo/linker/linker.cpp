#include "exceptions.hpp"
#include "linker.hpp"
#include <boost/algorithm/string/join.hpp>
#include <algorithm>
#include <iterator>
#include <list>
#include <sstream>
#include <stdexcept>
#include <vector>

Linker::Linker() {}

Linker::~Linker() {}

ObjectFileProcessing
Linker::link(ObjectFile const &obj_file)
{
    ObjectFileProcessing processing;
    processing.path = obj_file.path;
    processing.action = Action::PROCESSED;

    // Handle the exported symbols.
    for (Symbol const &s : obj_file.exported_symbols)
    {
        size_t c = m_exported.count(s);
        if (0 == c)
        {
            // The symbol is exported the first time.
            m_exported.insert(s);

            processing.exports.insert(s);
        }
        else if (1 == c)
        {
            throw MultipleDefinitions(s.name);
        }

        auto const &i = m_undefined.find(s);
        if (i != m_undefined.end())
        {
            // The symbol was in the imported table. Since now the symbol is
            // exported so it becomes defined, we can remove it from the list
            // of undefined symbols.
            m_undefined.erase(i);

            processing.resolves.insert(*i);
        }
    }

    // Handle the imported symbols.
    for (auto const &s : obj_file.imported_symbols)
    {
        size_t c = m_exported.count(s);
        if (0 == c)
        {
            // The imported symbol cannot be found in the already exported
            // symbols, so we need to add it.
            // It doesn't matter if the imported symbol is already in the
            // `m_undefined` list because `std::set` will automatically skip
            // the insertion if it already exists.
            m_undefined.insert(s);

            processing.requests.insert(s);
        }
        else if (1 == c)
        {
            // The symbol can be found in the exported symbol list so that
            // means this imported symbol is already defined somewhere else,
            // so we don't need to add it to the list of undefined symbols.
        }
    }

    m_processing.push_back(processing);

    return processing;
}

std::vector<ObjectFileProcessing>
Linker::link(StaticLib const &static_lib)
{
    // Whether an object file is added to the link during one scan. The
    // scanning will be stopped if no new object file is added to the link
    // after the scan; otherwise, the library will be scanned again.
    bool obj_file_added = false;

    // The object files that have been scanned. They won't be re-scanned when
    // the static library is re-scanned.
    std::set<ObjectFile> scanned;

    std::vector<ObjectFileProcessing> processings;

    do
    {
        // Reset the flag at the beginning of a scan.
        obj_file_added = false;

        // Go through every object file in the static library.
        for (ObjectFile const &f : static_lib.obj_files)
        {
            ObjectFileProcessing processing;
            processing.path = f.path;

            // If the object file had been scanned before, we won't scan it
            // again.
            if (scanned.find(f) != scanned.end())
            {
                processing.action = SKIPPED_SCANNED;
                processings.push_back(processing);
                m_processing.push_back(processing);
                continue;
            }

            if (!exports_undefined(f))
            {
                // If the current object file doesn't export any symbol that
                // is being requested, it is skipped in this scan.
                processing.action = SKIPPED_NO_EXPORTING_UNDEFINED;
                processings.push_back(processing);
                m_processing.push_back(processing);
                continue;
            }

            // OK, so now the object file exports some symbols that are being
            // requested. We need to link it.
            processing = this->link(f);
            processings.push_back(processing);

            // Remember this file so it won't be scanned again.
            scanned.insert(f);

            obj_file_added = true;
        }
    } while (obj_file_added);

    return processings;
}

std::string
Linker::str() const
{
    std::ostringstream oss;

    oss << "Linker"
        << "<";

    oss << "exported ["
        << boost::algorithm::join(symbol_set_to_string_vector(m_exported), ",")
        << "]";

    oss << "; ";

    oss << "undefined ["
        << boost::algorithm::join(symbol_set_to_string_vector(m_undefined), ",")
        << "]";

    oss << ">";

    return oss.str();
}

std::vector<ObjectFileProcessing>
Linker::get_all_processing_steps() const
{
    return m_processing;
}

ObjectFileProcessing
Linker::get_last_processing_step() const
{
    return m_processing.back();
}

std::set<Symbol>
Linker::report_undefined() const
{
    return m_undefined;
}

bool
Linker::exports_undefined(ObjectFile const &f) const
{
    // Get the intersection of the exported symbols and the currently undefined
    // symbols.
    std::list<Symbol> common;
    std::set_intersection(
        f.exported_symbols.begin(),
        f.exported_symbols.end(),
        m_undefined.begin(),
        m_undefined.end(),
        std::back_inserter(common));

    return !common.empty();
}
