#include <stdexcept>
#include <string>

/// Thrown when one symbol has multiple definitions
class MultipleDefinitions : public std::runtime_error
{
public:
    /// Constructor
    /// @param[in] symbol_name
    ///     The name of the symbol that has multiple definitions.
    MultipleDefinitions(std::string const &symbol_name);

    /// Copy constructor
    MultipleDefinitions(MultipleDefinitions const &) = default;

    /// Copy assignment operator
    MultipleDefinitions &
    operator=(MultipleDefinitions const &) = default;

    /// Move constructor
    MultipleDefinitions(MultipleDefinitions &&) = default;

    /// Move assignment operator
    MultipleDefinitions &
    operator=(MultipleDefinitions &&) = default;

    /// Destructor
    ~MultipleDefinitions() = default;

    std::string
    name() const;

private:
    std::string m_symbol_name;
};
