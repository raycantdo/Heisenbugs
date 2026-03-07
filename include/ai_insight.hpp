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
        SmartSuggestion<CourseType>&sugg )
    {
        if(sessions.empty())
            throw NoSession();

        if(courses.empty())
            throw NoData();

        std::cout << "\n---- AI Study Insights ----\n\n";
         bestStudyTime(sessions);
            weakSubject(courses);
            quizRiskWarning(sugg);
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
     static void quizRiskWarning(SmartSuggestion<CourseType>&sugg)
    {
        
       double studied=sugg.getfilluptime();
       std::string coursename=sugg.get_warningCourse();
        int remaining=sugg.getRemainingDays();
        if(coursename=="")
            throw NoQuiz();
        else{
                std::cout << "3. Quiz Risk Warning Insight:\n"
                          << "Your " << coursename
                          << " quiz is in " << remaining
                          << " days but you studied only "
                          << studied << " minutes.\n"
                          << "Consider revising today.\n\n";
        }
                
         
        
        
    }

};