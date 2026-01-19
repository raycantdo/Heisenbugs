#include "User.hpp"

int main()
{
    // 1. Create the User Account
    User student("Alice");

    // 2. User creates multiple profiles with different semester counts
    student.createProfile(Profile("Undergrad", 8));
    student.createProfile(Profile("Masters", 4));
    student.createProfile(Profile("Sum", 1));

    // 3. Display all profiles associated with this user
    student.displayProfiles();

    // 4. Switch between them
    std::cout << std::endl << "Switching to Graduate view:" << std::endl;
    student.switchProfile(1);

    return 0;
}