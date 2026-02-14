#include "Interface.hpp"
#include "DataHandler.hpp"
#include "StudySession.hpp"
#include "Quiz.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;

void displayTitle() {
    cout << R"(
  ____  _____ _   _ ______     ______ _   _ ____   ___  _   _ 
 / ___||_   _| | | |  _ \ \   / / ___| | | |  _ \ / _ \| \ | |
 \___ \  | | | | | | | | \ \ / / |   | |_| | |_) | | | |  \| |
  ___) | | | | |_| | |_| |\ V /| |___|  _  |  _ <| |_| | |\  |
 |____/  |_|  \___/|____/  |_|  \____|_| |_|_| \_\\___/|_| \_|
    )" << endl;
}

void studyPortal(User& activeUser)
{
    int userChoice;
    while (true)
    {
        cout << "\n======= DASHBOARD =======" << endl;
        cout << "1. View Stats\n2. Start Study\n3. Add Course\n4. Remove Course\n5. View Quizzes\n6. Exit\nChoice: ";
        
        if (!(cin >> userChoice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n'); 

        auto& courseList = activeUser.getProfiles()[0].getSemesters()[0].getCourses();

        if (userChoice == 1)
        {
            cout << "\n--- STUDY STATISTICS ---" << endl;
            if (StudySession::courseTotalTime.empty()) cout << "No sessions recorded yet." << endl;
            
            else
            {
                for (auto const& [course, time] : StudySession::courseTotalTime)
                {
                    cout << " > " << course << ": " << time << " minutes" << endl;
                }
            }
        } 
        else if (userChoice == 2)
        {
            cout << "Added Courses:" << endl;


            activeUser.getProfiles()[0].getSemesters()[0].displaySemesterInfo();


            string courseName; 
            cout << "Enter Course Name to Study: "; 
            getline(cin, courseName);

            // Check if course is in the list ---
            bool courseFound = false;
            for (const auto& course : courseList)
            {
                if (course.getCourseName() == courseName) {
                    courseFound = true;
                    break;
                }
            }

            if (courseFound)
            {
                StudySession session(courseName, MIDTERM, 1);
                session.startSession();
                cout << "Timer started for " << courseName << "! Press Enter to STOP..."; 
                cin.get(); 
                session.endSession();
                saveData(activeUser);
                cout << "Session saved successfully!" << endl;
            }
            else
            {
                cout << endl << "Error: You cannot study '" << courseName << "' because it is not in your course list." << endl;
                cout << "Please add the course first (Option 3)." << endl;
            }
        } 
        else if (userChoice == 3)
        {
            string name; float credits;
            cout << "New Course Name: "; getline(cin, name);
            cout << "Credit Hours: "; cin >> credits;
            cin.ignore(1000, '\n'); 
            

            activeUser.getProfiles()[0].getSemesters()[0].addCourse(name, credits);


            saveData(activeUser);
            cout << "Course added to your profile!" << endl;
        }
        else if (userChoice == 4)
        {
            string nameToRemove; 
            cout << "Enter Course to Remove: "; getline(cin, nameToRemove);


            bool courseWasFound = false;
            auto iterator = courseList.begin();

            while (iterator != courseList.end())
            {
                if (iterator->getCourseName() == nameToRemove)
                {

                    iterator = courseList.erase(iterator); 
                    
                    StudySession::courseTotalTime.erase(nameToRemove);
                    
                    courseWasFound = true;
                    cout << "Course removed successfully!" << endl;
                }
                else ++iterator;
            }

            if (courseWasFound) saveData(activeUser); // Save changes to files
            else cout << "Error: Course not found." << endl;
            
        }
        else if (userChoice == 5)
        {
            cout << "\n--- UPCOMING QUIZZES ---" << endl;

            for(const auto& course : courseList)
            {
                Quiz q(1, 1, course.getCourseName(), "Module Review", {2026, 2, 20, 14, 0});
                q.displayQuizInfo();
            }
        }
        else if (userChoice == 6) return;
    }
}