#include <iostream>
#include <cassert>
#include "Profile.hpp"

int main()
{
    Profile p1("Undergrad", 8);
    Profile p2("Masters", 4);
    Profile p3("PhD", 12);

    assert(p1.getName() == "Undergrad");
    assert(p2.getName() == "Masters");
    
    std::cout << "Profile logic tests passed." << std::endl;
    return 0;
}