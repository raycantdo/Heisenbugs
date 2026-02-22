#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "Quiz.hpp"
#include "StudySession.hpp"

template<typename CourseType>
class SmartSuggestion {
public:

    static CourseType findLeastStudied(
        const std::vector<CourseType>& courses,
        const std::map<std::string,double>& studyTime)
    {
        CourseType weakest = courses[0];
        double minTime = getTime(weakest, studyTime);

        for(const auto& c : courses){
            double t = getTime(c, studyTime);
            if(t < minTime){
                minTime = t;
                weakest = c;
            }
        }
        return weakest;
    }


    static std::vector<CourseType> upcomingQuizzes(
        const std::vector<CourseType>& courses,int semesterID)
    {
        std::vector<CourseType> result;

        for(const auto& c : courses){
            auto quizzes = loadQuizzes(semesterID, c.getCourseName());
            if(!quizzes.empty()) {   
                result.push_back(c);
            }
        }
        return result;
    }
     static void generateSuggestions(
        const std::vector<CourseType>& courses,
        int semesterID,
        const std::map<std::string,double>& studyTime)
    {
       

        
        auto weakCourse = findLeastStudied(courses, studyTime);
        std::cout << " You should study more: " 
                  << weakCourse.getCourseName() << "\n";

       
        auto quizCourses = upcomingQuizzes(courses, semesterID);
        std::vector<std::pair<CourseType, Quiz>> upcoming;
        for(const auto& c : quizCourses){
                auto quizzes = loadQuizzes(semesterID, c.getCourseName());
                for (auto& q : quizzes) {
                    upcoming.push_back({c, q});
                }
               
            }
        if(!upcoming.empty()){
            std::cout << "----Upcoming quizzes----\n";

            
             std::sort(upcoming.begin(), upcoming.end(), [](auto& a, auto& b){
                 tm aCopy = a.second.getQuizDate(); 
    tm bCopy = b.second.getQuizDate(); 
    return mktime(&aCopy) < mktime(&bCopy);
            });
            for (const auto& [c, q] : upcoming) {
                q.displayQuizInfo();

                
                double studied = 0.0;
                auto it = studyTime.find(c.getCourseName());
                if(it != studyTime.end()) studied = it->second;

                std::cout<<"You have studied it for "<<studied<<" minutes.\n";

            
        }
    }
    else {
        std::cout << "No upcoming quizzes found.\n";
    }
}

private:

    
    static double getTime(const CourseType& c,
                          const std::map<std::string,double>& studyTime)
    {
        auto it = studyTime.find(c.getCourseName());
        if(it == studyTime.end()) return 0.0; 
        return it->second;
    }
     static std::vector<Quiz> loadQuizzes(int semesterID, const std::string& courseName) {
        std::vector<Quiz> quizzes;
        std::ifstream file("quizzes.txt");
        if (!file) return quizzes;

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string quizIDStr,semStr, course, syllabus, dateStr, timeStr, doneStr;
            
            if (!std::getline(ss, quizIDStr, '|')) continue;
             if (!std::getline(ss, semStr, '|')) continue;
            if (!std::getline(ss, course, '|')) continue;
           
            if (!std::getline(ss, syllabus, '|')) continue;
            if (!std::getline(ss, dateStr, '|')) continue;
            if (!std::getline(ss, timeStr, '|')) continue;
            if (!std::getline(ss, doneStr)) continue;

            if (std::stoi(semStr) != semesterID) continue;
            if (course != courseName) continue;

            tm quizDate{};
            sscanf(dateStr.c_str(), "%d-%d-%d", &quizDate.tm_year, &quizDate.tm_mon, &quizDate.tm_mday);
            //quizDate.tm_year -= 1900; // tm_year stores years since 1900
            //quizDate.tm_mon -= 1;     // tm_mon is 0-11
            sscanf(timeStr.c_str(), "%d:%d", &quizDate.tm_hour, &quizDate.tm_min);

            Quiz q(std::stoi(quizIDStr), semesterID, course, syllabus, quizDate);
            if (doneStr == "1") q.markDone();

            quizzes.push_back(q);
        }
        return quizzes;
    }
};