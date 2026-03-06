#include "Quiz.hpp"
#include<fstream>
using namespace std;

int Quiz::getQuizID() const{
    return quizID;
}
string Quiz::getCourseName() const {
    return courseName;
}

string Quiz::getSyllabus() const {
    return syllabus;
}



void Quiz::saveQuizToFile(const Quiz& q)
{
    std::ifstream inFile("quizzes.txt");
    std::ofstream outFile("temp.txt");
    std::string line;
    std::string targetID = std::to_string(q.getQuizID()) + "|";

    
    if (inFile.is_open()) {
        while (std::getline(inFile, line)) {
            if (line.find(targetID) != 0 && !line.empty()) {
                outFile << line << "\n";
            }
        }
        inFile.close();
    }


    const tm& d = q.getQuizDate();
    outFile << q.getQuizID() << "|" << q.getSemesterId() << "|"
            << q.getCourseName() << "|" << q.getSyllabus() << "|"
            << d.tm_year << "-" << d.tm_mon << "-" << d.tm_mday << "|"
            << d.tm_hour << ":" << d.tm_min << "|"
            << q.isDone() << "\n";

    outFile.close();

    // Swap files
    std::remove("quizzes.txt");
    std::rename("temp.txt", "quizzes.txt");
}

void Quiz::addNewQuiz(int semesterID) {
    int id;
    string course, syllabus;
    int d, m, y, hr, min;

    cout << "\n--- SCHEDULE NEW QUIZ ---\n";
    cout << "Enter unique Quiz ID: "; cin >> id;
    cin.ignore(1000, '\n');
    
    cout << "Course Name: "; getline(cin, course);
    cout << "Syllabus/Topic: "; getline(cin, syllabus);
    
    cout << "Date (DD MM YYYY): "; cin >> d >> m >> y;
    cout << "Time (HH MM 24hr format): "; cin >> hr >> min;

    // tm_mon is 0-11, tm_year is years since 1900
    tm dateObj = {0};
    dateObj.tm_mday = d;
    dateObj.tm_mon = m - 1; 
    dateObj.tm_year = y - 1900;
    dateObj.tm_hour = hr;
    dateObj.tm_min = min;

    Quiz newQuiz(id, semesterID, course, syllabus, dateObj);
    Quiz::saveQuizToFile(newQuiz);
    cout << "Successfully added quiz for " << course << "!\n";
}