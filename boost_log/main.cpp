#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/utility/setup/from_settings.hpp>

void blah(void)
{
    boost::log::settings setts;
    boost::log::init_from_settings(setts);
}

int main(int, char**)
{
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    return EXIT_SUCCESS;
}
