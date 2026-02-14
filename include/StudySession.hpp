#ifndef STUDYSESSION_HPP
#define STUDYSESSION_HPP

#include <string>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <map>
#include "Course.hpp"
#include "Semester.hpp"
enum Term { MIDTERM, FINAL };

class StudySession:public Semester {
private:
    
    std::string courseName;
    time_t startTime;
    time_t endTime;
    bool isRunning;
    Term term;
    std::string semesterStartDate;

    
   
    
    
public:

    static std::map<std::string, double> courseTotalTime;          // course -> total hours
    static std::map<int, double> semesterTotalTime;                               // total semester hours
    static std::map<std::string, std::map<std::string, double>> dailyCourseTime; // course -> date -> hours

    static std::map<std::string, std::map<Term, double>> termCourseTime;        // course -> term -> hours

    StudySession(const std::string& name, Term t,int semID, const std::string& semesterStart = "2025-11-15")
        :Semester(semID), courseName(name), isRunning(false), term(t), semesterStartDate(semesterStart) {}

    void startSession() {
        startTime = time(nullptr);
        isRunning = true;
        std::cout << "Started studying " << courseName << std::endl;
    }

    void endSession() {
        if (!isRunning) {
            std::cout << "No active session.\n";
            return;
        }

        endTime = time(nullptr);
        isRunning = false;

        double minutes = getDurationInMinutes();
        std::string date = getCurrentDate();
       

       
        courseTotalTime[courseName] += minutes;
        semesterTotalTime[getSemesterId()] += minutes;
        dailyCourseTime[courseName][date] += minutes;
       
        termCourseTime[courseName][term] += minutes;

        std::cout << "Session ended: " << minutes << " minutes\n";
    }
    std::string getCurrentDate() const {
        time_t now = time(nullptr);
        tm* local = localtime(&now);

        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", local);
        return std::string(buffer);
    }

   


    double getDurationInMinutes() const {
        return difftime(endTime, startTime) / 60.0;
    }

   
    static double getCourseTotal(const std::string& course) {
        return courseTotalTime[course];
    }

    static double getDailyCourseTime(const std::string& course, const std::string& date) {
        return dailyCourseTime[course][date];
    }

   

    static double getTermCourseTime(const std::string& course, Term t) {
        return termCourseTime[course][t];
    }

    static double getSemesterTotal(int semID) {
        return semesterTotalTime[semID];
    }
};

#endif

