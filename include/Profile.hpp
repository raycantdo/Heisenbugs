#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <string>
#include <vector>

class Profile
{
private:
    std::string profileName;
    int totalSemesters;

public:
    Profile(std::string name, int semesters) : profileName(name), totalSemesters(semesters) {}

    std::string getName() const
    {
        return profileName;
    }
    void displayProfileInfo() const
    {
        std::cout << "Profile Name: " << profileName << ", Total Semesters: " << totalSemesters << std::endl;
    }
    
};

#endif