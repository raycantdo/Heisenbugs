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
#include "breakR.hpp"
#include "StudyPlan.hpp"
#include "StreakTracker.hpp"
#include "Routine.hpp"
#include "class.hpp"
#include "PomodoroTimer.hpp"
#include <ctime>

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
    auto& courseList = activeUser.getProfiles()[0].getSemesters()[0].getCourses();

    if (courseList.empty()) 
    {
        cout << "\nYou haven't added any courses yet. Please add a course first (Option 3).\n";
        return;
    }

    cout << "\n--- Available Courses ---\n";
    for (size_t i = 0; i < courseList.size(); ++i)
        cout << i + 1 << ". " << courseList[i].getCourseName() << "\n";

    cout << "Select a course to review flashcards (enter number): ";
    int choice;
    cin >> choice;
    cin.ignore(1000, '\n');

    if (choice < 1 || choice > static_cast<int>(courseList.size()))
    {
        cout << "Invalid choice.\n";
        return;
    }

    string courseName = courseList[choice - 1].getCourseName();

    FlashcardManager fm; 
    if (fm.loadForCourse(courseName)) 
        fm.runQuiz();
}

// ─────────────────────────────────────────────────────────────────────────────
// studyPlanPortal — full sub-menu for the StudyPlan feature
// ─────────────────────────────────────────────────────────────────────────────
void studyPlanPortal(User& activeUser)
{
    string studentName = activeUser.getProfiles()[0].getProfileName();
    StudyPlan plan("My Study Plan", studentName, 2);

    string planFile = studentName + "_studyplan.dat";
    try { plan.loadFromFile(planFile); }
    catch (...) {}

    int spChoice;
    do {
        plan.displaySummary();

        cout << "\n======= STUDY PLAN MANAGER =======" << endl;
        cout << " 1. Add Course to Plan\n";
        cout << " 2. List Courses\n";
        cout << " 3. Add Topic to a Course\n";
        cout << " 4. List All Topics\n";
        cout << " 5. List Topics by Course\n";
        cout << " 6. Schedule a Topic\n";
        cout << " 7. Mark Topic In Progress\n";
        cout << " 8. Mark Topic Complete\n";
        cout << "9. View Progress\n";
        cout << "10. Save Plan\n";
        cout << "11. Load Plan\n";
        cout << " 0. Back to Dashboard\n";
        cout << "Choice: ";

        cin >> spChoice;
        cin.ignore(1000, '\n');

        switch (spChoice)
        {
            case 1: {
                string cname;
                cout << "Course Name: "; getline(cin, cname);
                plan.addCourse(cname);
                cout << "Course added to plan!" << endl;
                break;
            }
            case 13: {
                plan.listCourses();
                cout << "Enter course number to remove: ";
                int idx; cin >> idx; cin.ignore(1000, '\n');
                plan.removeCourse(idx - 1);
                cout << "Course removed." << endl;
                break;
            }
            case 2: {
                plan.listCourses();
                break;
            }
            case 3: {
                string cname, tname;
                int hours;
                cout << "Course Name: "; getline(cin, cname);
                cout << "Topic Name: ";  getline(cin, tname);
                cout << "Estimated Hours: "; cin >> hours; cin.ignore(1000, '\n');
                try {
                    plan.addTopic(cname, tname, hours);
                    cout << "Topic added!" << endl;
                } catch (const runtime_error& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 4: {
                plan.listTopics();
                break;
            }
            case 5: {
                string cname;
                cout << "Course Name: "; getline(cin, cname);
                plan.listTopicsByCourse(cname);
                break;
            }
            case 6: {
                plan.listTopics();
                cout << "Enter topic number to schedule: ";
                int idx; cin >> idx; cin.ignore(1000, '\n');
                cout << "Enter date (YYYY-MM-DD): ";
                string dateStr; getline(cin, dateStr);
                try {
                    plan.scheduleTopic(idx - 1, Date(dateStr));
                } catch (const runtime_error& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 7: {
                plan.listTopics();
                cout << "Enter topic number to mark In Progress: ";
                int idx; cin >> idx; cin.ignore(1000, '\n');
                plan.markTopicInProgress(idx - 1);
                break;
            }
            case 8: {
                plan.listTopics();
                cout << "Enter topic number to mark Complete: ";
                int idx; cin >> idx; cin.ignore(1000, '\n');
                cout << "Actual hours spent: ";
                int hrs; cin >> hrs; cin.ignore(1000, '\n');
                try {
                    plan.markTopicComplete(idx - 1, hrs);
                } catch (const runtime_error& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 9: {
                cout << "\n--- PROGRESS REPORT ---" << endl;
                cout << "Overall: " << plan.getOverallProgress() << "%" << endl;
                cout << "Hours done: " << plan.getTotalStudyHours() << endl;
                cout << "Hours left: " << plan.getRemainingHours() << endl;
                break;
            }
            case 10: {
                string fname;
                cout << "Filename (default: " << planFile << "): ";
                getline(cin, fname);
                if (fname.empty()) fname = planFile;
                try { plan.saveToFile(fname); }
                catch (const runtime_error& e) { cout << "Error: " << e.what() << endl; }
                break;
            }
            case 11: {
                string fname;
                cout << "Filename to load: "; getline(cin, fname);
                try { plan.loadFromFile(fname); }
                catch (const runtime_error& e) { cout << "Error: " << e.what() << endl; }
                break;
            }
            case 0:
                try { plan.saveToFile(planFile); cout << "Plan auto-saved." << endl; }
                catch (...) {}
                cout << "Returning to dashboard..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }

    } while (spChoice != 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// streakTrackerPortal — sub-menu for the StreakTracker feature
// ─────────────────────────────────────────────────────────────────────────────
void streakTrackerPortal(StudyTracker::StreakTracker<>& tracker, const string& streakFile)
{
    int stChoice;
    do {
        tracker.display();

        cout << "\n======= STREAK TRACKER =======" << endl;
        cout << "1. Add Course to Tracker\n";
        cout << "2. Remove Course from Tracker\n";
        cout << "3. Record Study Day for a Course\n";
        cout << "4. View Current Streak\n";
        cout << "5. View Best Streak\n";
        cout << "0. Back to Dashboard\n";
        cout << "Choice: ";

        cin >> stChoice;
        cin.ignore(1000, '\n');

        switch (stChoice)
        {
            case 1: {
                string cname;
                cout << "Course Name to track: "; getline(cin, cname);
                try {
                    tracker.addCourse(cname);
                    cout << "Course added to streak tracker!" << endl;
                } catch (const StudyTracker::StreakException& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 2: {
                string cname;
                cout << "Course Name to remove: "; getline(cin, cname);
                try {
                    tracker.removeCourse(cname);
                    cout << "Course removed from streak tracker!" << endl;
                } catch (const StudyTracker::StreakException& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 3: {
                string cname;
                cout << "Course Name: "; getline(cin, cname);
                try {
                    tracker.recordStudyDay(cname);
                    cout << "Study day recorded! Current streak: "
                         << tracker.getCurrentStreak(cname) << " day(s)" << endl;
                } catch (const StudyTracker::StreakException& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 4: {
                string cname;
                cout << "Course Name: "; getline(cin, cname);
                try {
                    cout << "Current streak for " << cname << ": "
                         << tracker.getCurrentStreak(cname) << " day(s)" << endl;
                } catch (const StudyTracker::StreakException& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 5: {
                string cname;
                cout << "Course Name: "; getline(cin, cname);
                try {
                    cout << "Best streak for " << cname << ": "
                         << tracker.getBestStreak(cname) << " day(s)" << endl;
                } catch (const StudyTracker::StreakException& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 0:
                // ── Auto-save streaks when leaving sub-menu ──────────────────
                try { tracker.saveToFile(streakFile); cout << "Streaks auto-saved." << endl; }
                catch (const StudyTracker::StreakException& e) { cout << "Save error: " << e.what() << endl; }
                cout << "Returning to dashboard..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }

    } while (stChoice != 0);
}
// ─────────────────────────────────────────────────────────────────────────────


void studyPortal(User& activeUser)
{
    vector<StudySession> sessions;
    breakR b;
    ToDoList userToDo(activeUser.getProfiles()[0].getProfileName());
    StudyTracker::StreakTracker<> streakTracker;

    // ── Auto-load streaks on startup ─────────────────────────────────────────
    string studentName = activeUser.getProfiles()[0].getProfileName();
    string streakFile  = studentName + "_streaks.dat";
    try { streakTracker.loadFromFile(streakFile); }
    catch (...) { /* no existing file yet — start fresh */ }
    // ─────────────────────────────────────────────────────────────────────────
    SmartSuggestion<Course> sugg;
    int userChoice;
    while (true)
    {
        
        cout << "\n======= DASHBOARD =======" << endl;
        cout << "1. View Stats\n2. Start Study\n3. Add Course\n4. Remove Course\n5. Smart Suggestion \n6. Add Quiz\n7. Flashcards\n8. AI Study Insights \n9. To Do List\n10. Study Plan\n11. Streak Tracker\n12. Routine\n13. Exit\nChoice: ";
        
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
                    cout << " > " << course << ": " << time << " minutes" << endl;
            }
        } 
        // else if (userChoice == 2)
        // {
        //     cout << "Added Courses:" << endl;
        //     activeUser.getProfiles()[0].getSemesters()[0].displaySemesterInfo();

        //     string courseName; 
        //     cout << "Enter Course Name to Study: "; 
        //     getline(cin, courseName);

        //     bool courseFound = false;
        //     for (const auto& course : courseList)
        //     {
        //         if (course.getCourseName() == courseName) {
        //             courseFound = true;
        //             break;
        //         }
        //     }

        //     if (courseFound)
        //     {
        //         StudySession session(courseName, MIDTERM, 1);
        //         session.setBreakptr(&b);
        //         session.startSession();
        //         session.endSession();
        //         sessions.push_back(session);
        //         saveData(activeUser);
        //         cout << "Session saved successfully!\n" << endl;

        //         // ── Auto-record streak + auto-save ───────────────────────────
        //         try {
        //             try { streakTracker.addCourse(courseName); }
        //             catch (const StudyTracker::StreakException&) { /* already exists */ }

        //             streakTracker.recordStudyDay(courseName);
        //             cout << "🔥 Streak updated! Current streak for " << courseName
        //                  << ": " << streakTracker.getCurrentStreak(courseName)
        //                  << " day(s)" << endl;

        //             // Save immediately so streak isn't lost if program crashes
        //             streakTracker.saveToFile(streakFile);
        //         } catch (const StudyTracker::StreakException& e) {
        //             cout << "Streak note: " << e.what() << endl;
        //         }
        //         // ────────────────────────────────────────────────────────────
        //     }
        //     else
        //     {
        //         cout << endl << "Error: You cannot study '" << courseName << "' because it is not in your course list." << endl;
        //         cout << "Please add the course first (Option 3)." << endl;
        //     }
        // } 

        else if (userChoice == 2)
        {
            cout << "\n--- SELECT STUDY SESSION TYPE ---" << endl;
            cout << "1. Regular Study Session\n2. Pomodoro Timer\nChoice: ";
            int sessionType;
            if (!(cin >> sessionType) || (sessionType != 1 && sessionType != 2))
            {
                cout << "Invalid choice. Returning to dashboard." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            cin.ignore(1000, '\n');

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
                if(sessionType == 2) // Pomodoro
                {
                    TimerBase* timer = new PomodoroTimer();
                    timer->start(activeUser, courseName);
                    delete timer;
                }
                else if(sessionType == 1) // Regular
                {
                    StudySession session(courseName, MIDTERM, 1);
                    session.setBreakptr(&b);
                    session.startSession();
                    session.endSession();
                    sessions.push_back(session);
                    saveData(activeUser);
                    cout << "Session saved successfully!\n" << endl;
                }

                // ── Auto-record streak + auto-save ───────────────────────────
                try
                {
                    try { streakTracker.addCourse(courseName); }
                    catch (const StudyTracker::StreakException&) { }

                    streakTracker.recordStudyDay(courseName);
                    cout << "Streak updated! Current streak for " << courseName
                         << ": " << streakTracker.getCurrentStreak(courseName)
                         << " day(s)" << endl;

                    streakTracker.saveToFile(streakFile);
                }
                
                catch (const StudyTracker::StreakException& e)
                {
                    cout << "Streak note: " << e.what() << endl;
                }
            }
            else
            {
                cout << "Error: Course not found." << endl;
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

        else if (userChoice == 8)
        {
            try
            {
                AIInsight<Course>::generateInsights(sessions, courseList, sugg);
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
                        cout << "Task added successfully!" << endl;
                        break;
                    }
                    case 2: {
                        int id;
                        cout << "Enter Task ID to complete: "; cin >> id;
                        userToDo.mark_complete(id);
                        b.taskCompleted(userToDo.getCompletedTaskCount());
                        cout << "Task marked complete!" << endl;
                        break;
                    }
                    case 3: {
                        int id;
                        cout << "Enter Task ID to mark incomplete: "; cin >> id;
                        userToDo.mark_incomplete(id);
                        cout << "Task marked incomplete!" << endl;
                        break;
                    }
                    case 4: {
                        int id;
                        cout << "Enter Task ID to remove: "; cin >> id;
                        userToDo.remove_task(id);
                        cout << "Task removed!" << endl;
                        break;
                    }
                    case 5: {
                        int id, priority;
                        cout << "Enter Task ID: "; cin >> id;
                        cout << "Enter Priority (1=Low, 2=Medium, 3=High): "; cin >> priority;
                        if (priority >= 1 && priority <= 3) {
                            userToDo.set_priority(id, priority);
                            cout << "Priority updated!" << endl;
                        } else {
                            cout << "Invalid priority!" << endl;
                        }
                        break;
                    }
                    case 6: {
                        int id; double hours;
                        cout << "Enter Task ID: "; cin >> id;
                        cout << "Enter study hours to add: "; cin >> hours;
                        userToDo.add_study_time(id, hours);
                        cout << "Study time added!" << endl;
                        break;
                    }
                    case 7: {
                        int id; int days;
                        cout << "Enter Task ID: "; cin >> id;
                        cout << "Enter deadline (days from now): "; cin >> days;
                        time_t deadline = time(0) + (days * 24 * 60 * 60);
                        userToDo.add_deadline(id, deadline);
                        cout << "Deadline added!" << endl;
                        break;
                    }
                    case 8: {
                        int sortChoice;
                        cout << "Sort by:\n1. Priority\n2. Deadline\n3. Subject\nChoice: ";
                        cin >> sortChoice;
                        if (sortChoice == 1) userToDo.sort_by_priority();
                        else if (sortChoice == 2) userToDo.sort_by_deadline();
                        else if (sortChoice == 3) userToDo.sort_by_subject();
                        cout << "Tasks sorted!" << endl;
                        break;
                    }
                    case 9: {
                        vector<int> overdue = userToDo.get_overdue_tasks();
                        if (overdue.empty()) {
                            cout << "No overdue tasks!" << endl;
                        } else {
                            cout << "Overdue Tasks: ";
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
                            cout << "All tasks cleared!" << endl;
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
                            cout << "Error: " << e.what() << endl;
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
                            cout << "Error: " << e.what() << endl;
                        }
                        break;
                    }
                    case 0:
                        cout << "Returning to dashboard..." << endl;
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                }
            } while (todoChoice != 0);
        }

        else if (userChoice == 10)
        {
            studyPlanPortal(activeUser);
        }

        else if (userChoice == 11)
        {
            streakTrackerPortal(streakTracker, streakFile);
        }
        else if (userChoice == 12)
        {
            Routine<Quiz> examRoutine;
            auto quizzes = sugg.getAllQuizzes(courseList, semID);
            for (const auto &q : quizzes)
            {
                examRoutine.addItem(q);
            }
            time_t now = time(0);
            tm* ltm = localtime(&now);

            
            tm dateObj = *ltm;
            
            int d   = dateObj.tm_mday;
            int m   = dateObj.tm_mon + 1;    
            int y   = dateObj.tm_year + 1900;
            int hr  = dateObj.tm_hour;
            int min = dateObj.tm_min;

            cout << "Current Date and Time: " << d << "/" << m << "/" << y 
                << " " << hr << ":" << min << endl;
            examRoutine.addItem(Quiz(1, semID, "EEE", "Chap5", dateObj));
             time_t now = time(0);
            tm* ltm = localtime(&now);

            
            tm dateObj = *ltm;
            
            int d   = dateObj.tm_mday;
            int m   = dateObj.tm_mon + 1;    
            int y   = dateObj.tm_year + 1900;
            int hr  = dateObj.tm_hour;
            int min = dateObj.tm_min;

            cout << "Current Date and Time: " << d << "/" << m << "/" << y 
                << " " << hr << ":" << min << endl;
            
            while(1){
            string sub,syl;
            cin>>sub;
            if(sub=="\0")
                break;
            cin>>syl;

            examRoutine.addItem(Quiz(1, semID, sub, syl, dateObj));

            }
            Routine<ClassSession> classRoutine;
            while(1)
            {
                string c;
                
                int day,hour,minute;
                cin>>c;
                if(c=="\0")
                    break;
                cin>>day>>hour>>minute;
            classRoutine.addItem(ClassSession(c,day,hour,minute));
            }
            classRoutine.sortRoutine([](const ClassSession &a, const ClassSession &b)
                                     { return a.getday() < b.getday(); });
            cout<<"---------EXAM ROUTINE----------\n";
            examRoutine.showRoutine();
            cout<<"\n---------CLASS ROUTINE----------\n";
            classRoutine.showRoutine();
        }
        else if (userChoice == 13)  // Exit
        {
            userToDo.saveToFile("todo.dat");
            // ── Auto-save streaks on exit ────────────────────────────────────
            try { streakTracker.saveToFile(streakFile); }
            catch (...) {}
            // ─────────────────────────────────────────────────────────────────
            showHydrationReport(b);
            cout << "To-Do list and streaks saved. Goodbye!" << endl;
            return;
        }
        // else if (userChoice == 14) 
        // {
        //     cout << "\n--- Available Courses ---" << endl;
        //     for (const auto& course : courseList)
        //     {
        //         cout << "- " << course.getCourseName() << endl;
        //     }

        //     string selectedCourse;
        //     cout << "Select course for Pomodoro: ";
        //     getline(cin, selectedCourse);

            
        //     bool exists = false;
        //     for (const auto& c : courseList)
        //     {
        //         if (c.getCourseName() == selectedCourse) {
        //             exists = true;
        //             break;
        //         }
        //     }

        //     if (exists)
        //     {
        //         TimerBase* timer = new PomodoroTimer();
        //         timer->start(activeUser, selectedCourse);
        //         delete timer;
        //         // --- CONNECT TO STREAKS ---
        //         try
        //         {
        //             // Ensure course exists in tracker
        //             try { streakTracker.addCourse(selectedCourse); }
        //             catch (const StudyTracker::StreakException&) { /* already exists */ }

        //             streakTracker.recordStudyDay(selectedCourse);
                    
        //             cout << "\n🔥 Pomodoro Streak updated!" << endl;
        //             cout << "Current streak for " << selectedCourse << ": " 
        //                 << streakTracker.getCurrentStreak(selectedCourse) << " day(s)" << endl;

        //             // Save the streak to file immediately
        //             streakTracker.saveToFile(streakFile);
        //         }
        //         catch (const StudyTracker::StreakException& e)
        //         {
        //             cout << "Streak note: " << e.what() << endl;
        //         }
        //     }
        //     else
        //     {
        //         cout << "Error: Course not found in your list!" << endl;
        //     }
        // }
    }
}
