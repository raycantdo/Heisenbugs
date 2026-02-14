#include "User.hpp"
#include "Profile.hpp"
#include "Semester.hpp"
#include "Quiz.hpp"
#include "StudySession.hpp"
#include <fstream>

using namespace std;

#include <iomanip> // for setw
map<string, double> StudySession::courseTotalTime;
map<int, double> StudySession::semesterTotalTime  ;

map<string, map<string, double>> StudySession::dailyCourseTime;

map<string, map<Term, double>> StudySession::termCourseTime;

void syncUserToFile(const User& user)
{
    ofstream out("user_db.txt");
    out << user.getUserName() << endl;
    if (!user.getProfiles().empty()) {
        out << user.getProfiles()[0].getProfileName() << endl;
        for (const auto& sem : user.getProfiles()[0].getSemesters()) {
            for (const auto& course : sem.getCourses()) {
                out << course.getCourseName() << endl;
            }
        }
    }
}

int main()
{
    ifstream in("user_db.txt");

    string uName, pName;
    bool hasData = false;
    if (in && getline(in, uName) && !uName.empty())
    {
        if (getline(in, pName) && !pName.empty())
        {
            hasData = true;
        }
    }
    in.close();

    if (!hasData)
    {
        string name, p, c; float cr;
        cout << "--- REGISTER ---\nName: "; getline(cin, name); uName = name;
        cout << "Major: "; getline(cin, p);
        cout << "Course: "; getline(cin, c);
        cout << "Credits: "; cin >> cr; cin.ignore();
        User newUser(name); Profile prof(p, 8); Semester sem(1);
        sem.addCourse(c, cr); prof.addSemester(sem);
        newUser.createProfile(prof); syncUserToFile(newUser);
        
    }


    cout << "==========================================" << endl;
    cout << "Welcome, " << uName << "!" << endl;
    cout << "==========================================" << endl << endl;

    
    StudySession session1("Calculus I",FINAL,1);
    session1.startSession();
    int stop;
    cout << "Enter 0 to stop session: ";
    cin >> stop;
    if(!stop)
    session1.endSession();
    StudySession session2("Intro to Programming",MIDTERM,1);
    StudySession session3("Calculus I",MIDTERM,1);
    session2.startSession();
    cout << "Enter 0 to stop session: ";   
    cin >> stop;
    if(!stop)    session2.endSession();
    session3.startSession();
    cout << "Enter 0 to stop session: ";   
    cin >> stop;
    if(!stop)    session3.endSession();
    cout << "Duration of session 1: " << session1.getDurationInMinutes() << " minutes\n";
    cout << "Duration of session 2: " << session2.getDurationInMinutes() << " minutes\n";
    cout << "Duration of session 3: " << session3.getDurationInMinutes() << " minutes\n";
    string today = session1.getCurrentDate();
    cout << "Total time spent on Calculus I today: " 
              << StudySession::getDailyCourseTime("Calculus I", today) << " minutes\n";
    cout << "Total time spent on Intro to Programming today: " 
              << StudySession::getDailyCourseTime("Intro to Programming", today) << " minutes\n";
    cout<<"Total time spent on Calculus I :"<< StudySession::getCourseTotal("Calculus I") << " minutes\n";
    cout<<"Total time spent on Intro to Programming :"<< StudySession::getCourseTotal("Intro to Programming") << " minutes\n";
    cout << "Total time spent on Calculus I for MIDTERM: " 
              << StudySession::getTermCourseTime("Calculus I", MIDTERM) << " minutes\n";
    cout<<"Total time studying in Semester 1: "<< StudySession::getSemesterTotal(1) << " minutes\n";

    Quiz q1(1,1,"Calculus I","Differentiation",{2026,02,16,14,00});
    q1.displayQuizInfo();
    q1.setquizDate({2026,02,20,14,00});



    return 0;
}
