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
    ofstream file("quizzes.txt", ios::app);
    if (!file.is_open()) return;

    const tm& d = q.getQuizDate();

    file << q.getQuizID() << "|"
        <<q.getSemesterId()<<"|"
         << q.getCourseName() << "|"
         << q.getSyllabus() << "|"
         << (d.tm_year ) << "-"
         << (d.tm_mon ) << "-"
         << d.tm_mday << "|"
         << d.tm_hour << ":"
         << d.tm_min << "|"
         << q.isDone()
         << "\n";
         file.close();
}