#include "Interface.hpp"
#include "DataHandler.hpp"
#include "StudySession.hpp"
#include "Quiz.hpp"
#include "SmartSuggestion.hpp"
#include "Flashcard.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "ai_insight.hpp"
#include<fstream>
#include<sstream>
#include<iomanip>
#include <ToDoList.hpp>

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


 // reviewFlashcards(Helper Function) - Allows the user to review flashcards for a chosen course based on a course number
void reviewFlashcards(User& activeUser) 
{
    // Access the user's course list assuming the user has at least one course and one semester
    auto& courseList = activeUser.getProfiles()[0].getSemesters()[0].getCourses();

    if (courseList.empty()) 
    {
        cout << "\nYou haven't added any courses yet. Please add a course first (Option 3).\n";
        return;
    }

    //  Display courses with numbers for selection ---
    cout << "\n--- Available Courses ---\n";
    for (size_t i = 0; i < courseList.size(); ++i)
    {
        cout << i + 1 << ". " << courseList[i].getCourseName() << "\n";
    }

    //  Get user's choice ---
    cout << "Select a course to review flashcards (enter number): ";
    int choice;
    cin >> choice;          // Read the number
    cin.ignore(1000, '\n'); // Clear the newline left in the input buffer


    if (choice < 1 || choice > static_cast<int>(courseList.size()))
    {
        cout << "Invalid choice.\n";
        return;
    }

    // Extract the chosen course name ---
    string courseName = courseList[choice - 1].getCourseName(); // Convert to 0-based index


    FlashcardManager fm; 
    if (fm.loadForCourse(courseName)) 
    {
        // If loading succeeded, run the quiz ---
        fm.runQuiz();
    }
    // If loadForCourse() fails, it prints error message,
}


void studyPortal(User& activeUser)
{
    vector<StudySession>sessions;
    ToDoList userToDo(activeUser.getProfiles()[0].getProfileName());
    int userChoice;
    while (true)
    {
        SmartSuggestion<Course>sugg;
        cout << "\n======= DASHBOARD =======" << endl;
        cout << "1. View Stats\n2. Start Study\n3. Add Course\n4. Remove Course\n5. Smart Suggestion \n6. Add Quiz\n7. Flashcards\n8. AI Study Insights \n9. To Do List\n10. Exit\nChoice: ";
        
        if (!(cin >> userChoice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n'); 

        auto& courseList = activeUser.getProfiles()[0].getSemesters()[0].getCourses();
        int semID = activeUser.getProfiles()[0].getSemesters()[0].getSemesterId();
        
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
                sessions.push_back(session);
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

            if (courseWasFound) saveData(activeUser);
            else cout << "Error: Course not found." << endl;
            
        }
        
        else if (userChoice == 5) {
            sugg.generateSuggestions(courseList, semID, StudySession::getAllCourseTotals());
        }
  
        else if (userChoice == 6) {
            Quiz::addNewQuiz(semID);
        }

        else if (userChoice == 7) {
            reviewFlashcards(activeUser);
        }
        else if(userChoice==8)
        {
            try
            {
                AIInsight<Course>::generateInsights(sessions,courseList,sugg);
            }
            catch(const InsightException& e)
            {
                std::cerr << e.what() << '\n';
            }
            
        }

        else if (userChoice == 9) 
        {
            int todoChoice;
            cout << "\n--- TO-DO LIST MANAGER ---" << endl;
            userToDo.generate_progress_report();
            
            cout << "1. Add Task\n2. Mark Task Complete\n3. Remove Task\n4. Clear List\n5. Back to Dashboard\nChoice: ";
            cin >> todoChoice;
            cin.ignore(1000, '\n');

            

            if (todoChoice == 1)
            {
                string t, s, c;
                cout << "Task Title: "; getline(cin, t);
                cout << "Subject: "; getline(cin, s);
                cout << "Category (Assignment/Project/Exam): "; getline(cin, c);
                userToDo.add_task(t, s, c);
                cout << "Task added successfully!" << endl;
            }
            else if (todoChoice == 2)
            {
                int id;
                cout << "Enter Task ID to complete: "; cin >> id;
                userToDo.mark_complete(id);
            }
            else if (todoChoice == 3)
            {
                int id;
                cout << "Enter Task ID to remove: "; cin >> id;
                userToDo.remove_task(id);
            }
            else if (todoChoice == 4)
            {
                userToDo.clear_tasks();
                cout << "All tasks cleared!" << endl;
            }
            else if (todoChoice == 5)
            {
                continue; 
            }
            else
            {
                cout << "Invalid choice. Returning to dashboard." << endl;
            }
        }

        else if (userChoice == 10)
        {
            return;
        }
    }
}
