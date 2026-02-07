#ifndef QUIZ_HPP
#define QUIZ_HPP
#include <string>
#include<iostream>
#include <vector>
#include<ctime>
#include<sstream>
#include"Semester.hpp"
#include "Course.hpp"
using namespace std;
class Quiz:private Semester
{
    private:
    int quizID;
    string courseName;
    
    string syllabus;
    struct tm quizDate;
    public:
    Quiz(int id,int semID, string course, string syl, struct tm date) : Semester(semID), quizID(id), courseName(course), syllabus(syl), quizDate(date) {}
    void displayQuizInfo() const
    {
        cout << "Quiz ID: " << quizID << ", Course: " << courseName << ", Syllabus: " << syllabus 
             << ", Date: " << (quizDate.tm_year + 1900) << "-" << (quizDate.tm_mon + 1) << "-" << quizDate.tm_mday << endl;
        
        cout<<"Quiz time: "<<quizDate.tm_hour<<":"<<quizDate.tm_min<<endl;
        cout<<"Semester "<<getSemesterId()<<endl;
    }
    void setquizDate(struct tm date)
    {
        quizDate = date;
    }

};
#endif

    