#include "User.hpp"
#include "Profile.hpp"
#include "Semester.hpp"
#include "Quiz.hpp"
#include "StudySession.hpp"

#include <iomanip> // for std::setw
std::map<std::string, double> StudySession::courseTotalTime;
std::map<int, double> StudySession::semesterTotalTime  ;

std::map<std::string, std::map<std::string, double>> StudySession::dailyCourseTime;

std::map<std::string, std::map<Term, double>> StudySession::termCourseTime;
int main()
{
    // 1. Create the User Account
    User user("Alice");

    std::cout << "==========================================" << std::endl;
    std::cout << "Welcome, " << user.getUserName() << "!" << std::endl;
    std::cout << "==========================================" << std::endl << std::endl;

    // 2. Create a profile for undergraduate studies
    Profile undergradProfile("Undergrad", 8);

    // Add courses to Semester 1
    Semester semester1(1);
    semester1.addCourse(Course("Calculus I", 4.0));
    semester1.addCourse(Course("Intro to Programming", 3.0));

    // Link the semester to the profile
    undergradProfile.addSemester(semester1);

    // Add the profile to the user
    user.createProfile(undergradProfile);

    // Display courses for Semester 1
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Courses in Semester 1 of Profile: " 
              << undergradProfile.getProfileName() << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    undergradProfile.displayProfileInfo();
    
    StudySession session1("Calculus I",FINAL,1);
    session1.startSession();
    int stop;
    std::cout << "Enter 0 to stop session: ";
    std::cin >> stop;
    if(!stop)
    session1.endSession();
    StudySession session2("Intro to Programming",MIDTERM,1);
    StudySession session3("Calculus I",MIDTERM,1);
    session2.startSession();
    std::cout << "Enter 0 to stop session: ";   
    std::cin >> stop;
    if(!stop)    session2.endSession();
    session3.startSession();
    std::cout << "Enter 0 to stop session: ";   
    std::cin >> stop;
    if(!stop)    session3.endSession();
    std::cout << "Duration of session 1: " << session1.getDurationInMinutes() << " minutes\n";
    std::cout << "Duration of session 2: " << session2.getDurationInMinutes() << " minutes\n";
    std::cout << "Duration of session 3: " << session3.getDurationInMinutes() << " minutes\n";
    std::string today = session1.getCurrentDate();
    std::cout << "Total time spent on Calculus I today: " 
              << StudySession::getDailyCourseTime("Calculus I", today) << " minutes\n";
    std::cout << "Total time spent on Intro to Programming today: " 
              << StudySession::getDailyCourseTime("Intro to Programming", today) << " minutes\n";
    std::cout<<"Total time spent on Calculus I :"<< StudySession::getCourseTotal("Calculus I") << " minutes\n";
    std::cout<<"Total time spent on Intro to Programming :"<< StudySession::getCourseTotal("Intro to Programming") << " minutes\n";
    std::cout << "Total time spent on Calculus I for MIDTERM: " 
              << StudySession::getTermCourseTime("Calculus I", MIDTERM) << " minutes\n";
    std::cout<<"Total time studying in Semester 1: "<< StudySession::getSemesterTotal(1) << " minutes\n";

    Quiz q1(1,1,"Calculus I","Differentiation",{2026,02,16,14,00});
    q1.displayQuizInfo();
    q1.setquizDate({2026,02,20,14,00});

    // Add other profiles
    user.createProfile(Profile("Masters", 4));
    user.createProfile(Profile("Summer", 1));

    // Display all profiles associated with the user
    std::cout << std::endl << "==========================================" << std::endl;
    std::cout << "All Profiles of " << user.getUserName() << ":" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;
    user.displayProfiles();

    // Switch to a different profile (Graduate)
    std::cout << std::endl << "==========================================" << std::endl;
    std::cout << "Switching to Graduate Profile view:" << std::endl;
    std::cout << "==========================================" << std::endl;
    user.switchProfile(1); // assuming index 1 is Masters



    return 0;
}
