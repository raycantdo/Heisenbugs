#include <iostream>
#include <cassert>
#include "User.hpp"

int main() {
    User u1("Sohan");
    User u2("Labeeb");

    // Test unique ID logic
    assert(u1.getUserId() != u2.getUserId());
    
    // Test workflow
    u1.createProfile(Profile("Bachelors", 8));
    u1.createProfile(Profile("Masters", 4));
    u1.createProfile(Profile("Summer", 1));
    
    u1.deleteProfile(0);
    // u1 should now only have 2 profiles
    
    std::cout << "User logic tests passed." << std::endl;
    return 0;
}