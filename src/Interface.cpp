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
#include "ToDoList.hpp"
 

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
        cout << "1. View Stats\n2. Start Study\n3. Add Course\n4. Remove Course\n5. Smart Suggestion \n6. Add Quiz\n7. Flashcards\n8. AI Study Insights \n9. To Do List\n10. File Options\n11. Exit\nChoice: ";
        
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
            do {
                cout << "\n========== TO-DO LIST MANAGER ==========" << endl;
                userToDo.generate_progress_report();
                
                cout << "\n--- TASK MANAGEMENT ---" << endl;
                cout << "1. Add Task" << endl;
                cout << "2. Mark Task Complete" << endl;
                cout << "3. Mark Task Incomplete" << endl;
                cout << "4. Remove Task" << endl;
                cout << "5. Set Priority (1=Low, 2=Medium, 3=High)" << endl;
                cout << "6. Add Study Time" << endl;
                cout << "7. Add Deadline" << endl;
                cout << "8. Sort Tasks" << endl;
                cout << "9. Show Overdue Tasks" << endl;
                cout << "10. Clear All Tasks" << endl;
                
                cout << "\n--- FILE HANDLING ---" << endl;
                cout << "11. Save To-Do List" << endl;
                cout << "12. Load To-Do List" << endl;
                cout << "13. Export as CSV" << endl;
                cout << "14. Save as Simple Format" << endl;
                cout << "15. Load Simple Format" << endl;
                
                cout << "\n0. Back to Dashboard" << endl;
                cout << "Choice: ";
                
                cin >> todoChoice;
                cin.ignore(1000, '\n');

                switch(todoChoice) {
                    case 1: {
                        string t, s, c;
                        cout << "Task Title: "; getline(cin, t);
                        cout << "Subject: "; getline(cin, s);
                        cout << "Category (Assignment/Project/Exam): "; getline(cin, c);
                        userToDo.add_task(t, s, c);
                        cout << "✅ Task added successfully!" << endl;
                        break;
                    }
                    case 2: {
                        int id;
                        cout << "Enter Task ID to complete: "; cin >> id;
                        userToDo.mark_complete(id);
                        cout << "✅ Task marked complete!" << endl;
                        break;
                    }
                    case 3: {
                        int id;
                        cout << "Enter Task ID to mark incomplete: "; cin >> id;
                        userToDo.mark_incomplete(id);
                        cout << "✅ Task marked incomplete!" << endl;
                        break;
                    }
                    case 4: {
                        int id;
                        cout << "Enter Task ID to remove: "; cin >> id;
                        userToDo.remove_task(id);
                        cout << "✅ Task removed!" << endl;
                        break;
                    }
                    case 5: {
                        int id, priority;
                        cout << "Enter Task ID: "; cin >> id;
                        cout << "Enter Priority (1=Low, 2=Medium, 3=High): "; cin >> priority;
                        if (priority >= 1 && priority <= 3) {
                            userToDo.set_priority(id, priority);
                            cout << "✅ Priority updated!" << endl;
                        } else {
                            cout << "❌ Invalid priority!" << endl;
                        }
                        break;
                    }
                    case 6: {
                        int id; double hours;
                        cout << "Enter Task ID: "; cin >> id;
                        cout << "Enter study hours to add: "; cin >> hours;
                        userToDo.add_study_time(id, hours);
                        cout << "✅ Study time added!" << endl;
                        break;
                    }
                    case 7: {
                        int id; int days;
                        cout << "Enter Task ID: "; cin >> id;
                        cout << "Enter deadline (days from now): "; cin >> days;
                        time_t deadline = time(0) + (days * 24 * 60 * 60);
                        userToDo.add_deadline(id, deadline);
                        cout << "✅ Deadline added!" << endl;
                        break;
                    }
                    case 8: {
                        int sortChoice;
                        cout << "Sort by:\n1. Priority\n2. Deadline\n3. Subject\nChoice: ";
                        cin >> sortChoice;
                        if (sortChoice == 1) userToDo.sort_by_priority();
                        else if (sortChoice == 2) userToDo.sort_by_deadline();
                        else if (sortChoice == 3) userToDo.sort_by_subject();
                        cout << "✅ Tasks sorted!" << endl;
                        break;
                    }
                    case 9: {
                        vector<int> overdue = userToDo.get_overdue_tasks();
                        if (overdue.empty()) {
                            cout << "🎉 No overdue tasks!" << endl;
                        } else {
                            cout << "⚠️ Overdue Tasks: ";
                            for (int id : overdue) cout << id << " ";
                            cout << endl;
                        }
                        break;
                    }
                    case 10: {
                        char confirm;
                        cout << "Are you sure? (y/n): "; cin >> confirm;
                        if (confirm == 'y' || confirm == 'Y') {
                            userToDo.clear_tasks();
                            cout << "✅ All tasks cleared!" << endl;
                        }
                        break;
                    }
                    case 11: {
                        string filename;
                        cout << "Enter filename to save (default: todo.dat): ";
                        getline(cin, filename);
                        if (filename.empty()) filename = "todo.dat";
                        userToDo.saveToFile(filename);
                        break;
                    }
                    case 12: {
                        string filename;
                        cout << "Enter filename to load: "; getline(cin, filename);
                        try {
                            userToDo.loadFromFile(filename);
                        } catch (const exception& e) {
                            cout << "❌ Error: " << e.what() << endl;
                        }
                        break;
                    }
                    case 13: {
                        string filename;
                        cout << "Enter CSV filename (default: todo.csv): ";
                        getline(cin, filename);
                        if (filename.empty()) filename = "todo.csv";
                        userToDo.exportToCSV(filename);
                        break;
                    }
                    case 14: {
                        string filename;
                        cout << "Enter filename for simple format: ";
                        getline(cin, filename);
                        userToDo.saveToSimpleFormat(filename);
                        break;
                    }
                    case 15: {
                        string filename;
                        cout << "Enter simple format filename to load: ";
                        getline(cin, filename);
                        try {
                            userToDo.loadFromSimpleFormat(filename);
                        } catch (const exception& e) {
                            cout << "❌ Error: " << e.what() << endl;
                        }
                        break;
                    }
                    case 0:
                        cout << "Returning to dashboard..." << endl;
                        break;
                    default:
                        cout << "❌ Invalid choice!" << endl;
                }
            } while (todoChoice != 0);
        }

        else if (userChoice == 10)  // File Options
        {
            int fileChoice;
            cout << "\n========== FILE OPTIONS ==========" << endl;
            cout << "1. Save To-Do List" << endl;
            cout << "2. Load To-Do List" << endl;
            cout << "3. Export as CSV" << endl;
            cout << "4. Back to Dashboard" << endl;
            cout << "Choice: ";
            cin >> fileChoice;
            cin.ignore(1000, '\n');
            
            if (fileChoice == 1) {
                string filename;
                cout << "Enter filename to save: ";
                getline(cin, filename);
                userToDo.saveToFile(filename);
            }
            else if (fileChoice == 2) {
                string filename;
                cout << "Enter filename to load: ";
                getline(cin, filename);
                try {
                    userToDo.loadFromFile(filename);
                } catch (const exception& e) {
                    cout << "❌ Error: " << e.what() << endl;
                }
            }
            else if (fileChoice == 3) {
                string filename;
                cout << "Enter CSV filename: ";
                getline(cin, filename);
                userToDo.exportToCSV(filename);
            }
        }
        else if (userChoice == 11)  // Exit
        {
            // Auto-save before exit
            userToDo.saveToFile("todo.dat");
            cout << "✅ To-Do list saved. Goodbye!" << endl;
            return;
        }
    }
}
