#include "DataHandler.hpp"
#include "StudySession.hpp"
#include <fstream>

void saveData(const User& activeUser)
{
    ofstream userFile("user_db.txt");
    if (userFile.is_open())
    {
        userFile << activeUser.getUserName() << endl;
        if (!activeUser.getProfiles().empty())
        {
            userFile << activeUser.getProfiles()[0].getProfileName() << endl;
            for (const auto& semester : activeUser.getProfiles()[0].getSemesters())
            {
                for (const auto& course : semester.getCourses())
                {
                    userFile << course.getCourseName() << endl;
                }
            }
        }
        userFile.close();
    }

    ofstream sessionFile("sessions_db.txt");
    if (sessionFile.is_open())
    {
        for (auto const& [name, minutes] : StudySession::courseTotalTime)
        {
            sessionFile << name << "," << minutes << endl;
        }
        sessionFile.close();
    }
}

void loadSessionData()
{
    ifstream sessionFile("sessions_db.txt");
    string courseName, timeString;
    
    StudySession::courseTotalTime.clear();

    // Read the line up to the comma, then the rest into timeString
    while (getline(sessionFile, courseName, ',') && getline(sessionFile, timeString))
    {
        if (!courseName.empty() && !timeString.empty())
        {
            StudySession::courseTotalTime[courseName] = stod(timeString);
        }
    }
}
