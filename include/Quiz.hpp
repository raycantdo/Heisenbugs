#ifndef QUIZ_HPP
#define QUIZ_HPP
#include <string>
#include<iostream>
#include <vector>
#include<ctime>
#include<sstream>
#include<map>
#include<fstream>
#include"Semester.hpp"
#include "Course.hpp"
using namespace std;
class Quiz:private Semester
{
    private:
    int quizID;
    string courseName;
    bool done;
    string syllabus;
    struct tm quizDate;

    public:
    Quiz(int id,int semID, string course, string syl, struct tm date) : Semester(semID), quizID(id), courseName(course), syllabus(syl), quizDate(date) ,done(false)
    {
        
    }
    void displayQuizInfo() const
    {
        cout << "Quiz ID: " << quizID << "\nCourse: " << courseName << "\nSyllabus: " << syllabus 
             << "\nDate: " << (quizDate.tm_year) << "-" << (quizDate.tm_mon) << "-" << quizDate.tm_mday << endl;
        
        cout<<"Quiz time: "<<quizDate.tm_hour<<":"<<quizDate.tm_min<<endl;
        cout<<"Semester "<<getSemesterId()<<endl;
       
    }
    void setquizDate(struct tm date)
    {
        quizDate = date;
    }
    int getQuizID() const;
    string getCourseName() const;
    string getSyllabus() const;
    const tm& Quiz::getQuizDate() const{
    return quizDate;   
}
    void markDone() { done = true; }
    bool isDone() const { return done; }
    static void saveQuizToFile(const Quiz& q);

};
#endif

    