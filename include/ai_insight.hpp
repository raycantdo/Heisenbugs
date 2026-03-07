#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include "StudySession.hpp"
#include "SmartSuggestion.hpp"
#include "FocusTimeAnalyzer.hpp"
#include "Quiz.hpp"
#include "InsightException.hpp"
#include "NoSession.hpp"
#include "NoQuiz.hpp"
#include "NoData.hpp"
template<typename CourseType>
class AIInsight
{
public:

    static void generateInsights(
        const std::vector<StudySession>& sessions,
        const std::vector<CourseType>& courses,
        int semesterID)
    {
        if(sessions.empty())
            throw NoSessionLogged();

        if(courses.empty())
            throw NoDataFound();

        std::cout << "\n---- AI Study Insights ----\n\n";
         bestStudyTime(sessions);
            weakSubject(courses);
            quizRiskWarning(courses, semesterID);
    }
    private:
    

   
    static void bestStudyTime(const std::vector<StudySession>& sessions)
    {
         std::cout <<
        "1. Best Study Time Insight:\nYour ";
        FocusTimeAnalyzer focus;
        focus.analyze(sessions);

       
        
        std::cout<<"Consider scheduling difficult subjects then.\n\n";
    }
    static void weakSubject(const std::vector<CourseType>& courses)
    {
        auto totals = StudySession::getAllCourseTotals();

        if(totals.empty()) 
        {
            throw NoData();
        }
        auto weakest=SmartSuggestion<CourseType>::findLeastStudied(courses,totals);
        std::cout <<
        "2. Weak Subject Insight:\n"<<
        "You spent only "
        << StudySession::getCourseTotal(weakest.getCourseName())
        << " minutes on "
        << weakest.getCourseName()
        << ".\n"
        "It is currently your least studied subject.\n\n";
    }
     static void quizRiskWarning(
        const std::vector<CourseType>& courses,
        int semesterID)
    {
        bool found=false;
        auto totals = StudySession::getAllCourseTotals();
        time_t now = time(nullptr);

        for(const auto& c : courses)
        {
            auto quizzes =
                SmartSuggestion<CourseType>::loadQuizzes(
                    semesterID, c.getCourseName());

            for(const auto& q : quizzes)
            {
                tm date = q.getQuizDate();
                time_t quizTime = mktime(&date);

                double days =
                    difftime(quizTime, now) / (60*60*24);

                if(days <= 3 && days >= 0)
                {
                    double studied =
                        StudySession::getCourseTotal(
                            c.getCourseName());

                    if(studied < 30)
                    {
                        std::cout <<
                        "3.Quiz Risk Warning Insight:\n"
                        "Your " << c.getCourseName()
                        << " quiz is in "
                        << (int)days
                        << " days but you studied only "
                        << studied
                        << " minutes.\n"
                        "Consider revising today.\n\n";
                        found=true;
                    }
                }
            }
        }
        if(!found)
            throw NoQuiz();
    }

};