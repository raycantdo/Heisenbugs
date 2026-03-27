#ifndef PROFILE_HPP
#define PROFILE_HPP

#include "Semester.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
class Profile
{
private:
    string profileName;
    int totalSemesters;
    vector<Semester> semesters;

public:
    Profile(std::string name, int semestersCount) : profileName(name), totalSemesters(semestersCount) {}


    string getProfileName() const
    {
        return profileName;
    }
    void addSemester(const Semester& sem)
    {
        semesters.push_back(sem);
    }

    void displayProfileInfo() const
    {
        cout << "Profile Name: " << profileName << ", Total Semesters: " << totalSemesters << endl;
        for (const auto& sem : semesters)
        {
            sem.displaySemesterInfo();
        }
    }
    vector<Semester>& getSemesters()
    {
        return semesters;
    }
    const vector<Semester>& getSemesters() const
    {
        return semesters;
    }
    
};
#endif