#include "User.hpp"
#include "Profile.hpp"
#include "Semester.hpp"
#include <iomanip> // for std::setw

int main()
{
    // 1. Create the User Account
    User user("Alice");

    std::cout << "==========================================" << std::endl;
    std::cout << "Welcome, " << user.getUserName() << "!" << std::endl;
    std::cout << "==========================================" << std::endl << std::endl;

    // 2. Create a profile for undergraduate studies
    Profile undergradProfile("Undergrad", 8);

    // Add courses to Semester 1
    Semester semester1(1);
    semester1.addCourse(Course("Calculus I", 4.0));
    semester1.addCourse(Course("Intro to Programming", 3.0));

    // Link the semester to the profile
    undergradProfile.addSemester(semester1);

    // Add the profile to the user
    user.createProfile(undergradProfile);

    // Display courses for Semester 1
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Courses in Semester 1 of Profile: " 
              << undergradProfile.getProfileName() << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    undergradProfile.displayProfileInfo();

    // Add other profiles
    user.createProfile(Profile("Masters", 4));
    user.createProfile(Profile("Summer", 1));

    // Display all profiles associated with the user
    std::cout << std::endl << "==========================================" << std::endl;
    std::cout << "All Profiles of " << user.getUserName() << ":" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;
    user.displayProfiles();

    // Switch to a different profile (Graduate)
    std::cout << std::endl << "==========================================" << std::endl;
    std::cout << "Switching to Graduate Profile view:" << std::endl;
    std::cout << "==========================================" << std::endl;
    user.switchProfile(1); // assuming index 1 is Masters

    return 0;
}
