#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "User.hpp"
#include "Profile.hpp"
#include "Semester.hpp"
#include "StudySession.hpp"
#include "Quiz.hpp"
#include "DataHandler.hpp"
#include "Interface.hpp"

using namespace std;

// Define static members for the linker
map<string, double> StudySession::courseTotalTime;
map<int, double> StudySession::semesterTotalTime;
map<string, map<string, double>> StudySession::dailyCourseTime;
map<string, map<Term, double>> StudySession::termCourseTime;


int main()
{
    displayTitle();

    loadSessionData(); // Load the global study times map
    
    ifstream userDB("user_db.txt");
    string userName, majorName, profileName; int totalSemesterCount;
    vector<string> savedCourses;
    
    // 1. Better Loading Logic
    if (getline(userDB, userName) && getline(userDB, majorName)) {
        string courseName;
        while (getline(userDB, courseName)) {
            if(!courseName.empty()) savedCourses.push_back(courseName);
        }
    }
    userDB.close();

    // 2. Setup the User object
    User activeUser(userName.empty() ? "Guest" : userName);
    
    if (userName.empty()) {
        // --- REGISTRATION MODE ---
        cout << "--- REGISTER ---\nName: "; getline(cin, userName);
        cout << "Major: "; getline(cin, majorName);
        cout << "Profile: "; getline(cin, profileName);
        cout << "Total Semesters: "; cin >> totalSemesterCount;
        
        activeUser.setUserName(userName);
        Profile newProfile(profileName, totalSemesterCount);
        newProfile.addSemester(Semester(1));
        activeUser.createProfile(newProfile);
        
        saveData(activeUser);
    }
    else
    {
        // --- LOGIN MODE (Rebuilding the saved profile) ---
        Profile existingProfile(profileName, totalSemesterCount);
        Semester firstSemester(1);
        
        // Re-add the courses we found in the file
        for (const string& cName : savedCourses)
        {
            firstSemester.addCourse(cName, 3.0); // Assuming 3 credits default
        }
        
        existingProfile.addSemester(firstSemester);
        activeUser.createProfile(existingProfile);
        
        cout << "Welcome back, " << userName << "!" << endl;
    }

    studyPortal(activeUser);
    return 0;
}