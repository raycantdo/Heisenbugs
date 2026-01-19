#ifndef PROFILE_HPP
#define PROFILE_HPP

#include "Semester.hpp"
#include <string>
#include <vector>
#include <iostream>
class Profile
{
private:
    std::string profileName;
    int totalSemesters;
    std::vector<Semester> semesters;

public:
    Profile(std::string name, int semestersCount) : profileName(name), totalSemesters(semestersCount) {}


    std::string getProfileName() const
    {
        return profileName;
    }
    void addSemester(const Semester& sem)
    {
        semesters.push_back(sem);
    }

    void displayProfileInfo() const
    {
        std::cout << "Profile Name: " << profileName << ", Total Semesters: " << totalSemesters << std::endl;
        for (const auto& sem : semesters)
        {
            sem.displaySemesterInfo();
        }
        //std::cout << std::endl;
    }
};
#endif