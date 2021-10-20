#include "simplecalc.hpp"
#include "simplestr.hpp"
#include <iostream>
#include <string>

int main(int argc, char * argv[])
{
    std::cout << "1 + 1 = " << simplecalc::add(1, 1) << std::endl;
    std::cout << "2 - 1 = " << simplecalc::subtract(2, 1) << std::endl;

    std::string s1("abc123ABC");
    std::cout << "String \"" << s1 << "\" is all alphanumeric: "
        << simplestr::is_all_alphanumeric(s1) << std::endl;

    std::string s2("abc+-*/_123ABC");
    std::cout << "String \"" << s2 << "\" is all alphanumeric: "
        << simplestr::is_all_alphanumeric(s2) << std::endl;

    return 0;
}
