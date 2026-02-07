#include <iostream>
#include <thread>
#include "StudySession.hpp"
std::map<std::string, double> StudySession::courseTotalTime;
std::map<int, double> StudySession::semesterTotalTime  ;

std::map<std::string, std::map<std::string, double>> StudySession::dailyCourseTime;

std::map<std::string, std::map<Term, double>> StudySession::termCourseTime;
int main() {
    std::cout << "=== Study Session Test Started ===\n\n";

    
    StudySession s1("CSE2201", MIDTERM,1);
    StudySession s2("CSE2201", FINAL,1);
    StudySession s3("MAT2101", MIDTERM,1);

    
    s1.startSession();
    int stop;
    std::cout << "Enter 0 to stop session: ";
    std::cin >> stop;
    if(!stop)
    s1.endSession();

    
    s2.startSession();
    
    std::cout << "Enter 0 to stop session: ";
    std::cin >> stop;
    if(!stop)
    s2.endSession();

    s3.startSession();
    std::cin >> stop;
    if(!stop)
    s3.endSession();

    std::cout << "\n=== Summary ===\n";

    
    std::cout << "CSE2201 Total Minutes: "
              << StudySession::getCourseTotal("CSE2201") << "\n";

    std::cout << "MAT2101 Total Minutes: "
              << StudySession::getCourseTotal("MAT2101") << "\n";

    std::string today = s1.getCurrentDate();
    std::cout << "CSE2201 Today (" << today << "): "
              << StudySession::getDailyCourseTime("CSE2201", today) << "\n";

    

 
    std::cout << "CSE2201 MIDTERM Minutes: "
              << StudySession::getTermCourseTime("CSE2201", MIDTERM) << "\n";

    std::cout << "CSE2201 FINAL Minutes: "
              << StudySession::getTermCourseTime("CSE2201", FINAL) << "\n";

 
    std::cout << "Total Semester Study Minutes: "
              << StudySession::getSemesterTotal(1) << "\n";

    std::cout << "\n=== Test Finished ===\n";

    return 0;
}
