#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "Quiz.hpp"
#include "StudySession.hpp"
#include <ctime>
#include "Routine.hpp"
template <typename CourseType>

class SmartSuggestion
{
private:
    string warning_course;
    double fillup_time;
    int remainingDay;

    static double getTime(const CourseType &c,
                          const std::map<std::string, double> &studyTime)
    {
        auto it = studyTime.find(c.getCourseName());
        if (it == studyTime.end())
            return 0.0;
        return it->second;
    }
    static std::vector<Quiz> loadQuizzes(int semesterID, const std::string &courseName)
    {
        std::vector<Quiz> quizzes;
        std::ifstream file("quizzes.txt");
        if (!file)
            return quizzes;

        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string quizIDStr, semStr, course, syllabus, dateStr, timeStr, doneStr;

            if (!std::getline(ss, quizIDStr, '|'))
                continue;
            if (!std::getline(ss, semStr, '|'))
                continue;
            if (!std::getline(ss, course, '|'))
                continue;
            if (!std::getline(ss, syllabus, '|'))
                continue;
            if (!std::getline(ss, dateStr, '|'))
                continue;
            if (!std::getline(ss, timeStr, '|'))
                continue;
            if (!std::getline(ss, doneStr))
                continue;

            if (std::stoi(semStr) != semesterID)
                continue;
            if (course != courseName)
                continue;

            tm quizDate{};
            // Parse date
            sscanf(dateStr.c_str(), "%d-%d-%d", &quizDate.tm_year, &quizDate.tm_mon, &quizDate.tm_mday);

            // Convert back to tm standards
            quizDate.tm_year -= 1900; // tm_year: years since 1900
            quizDate.tm_mon -= 1;     // tm_mon: 0-11

            sscanf(timeStr.c_str(), "%d:%d", &quizDate.tm_hour, &quizDate.tm_min);

            Quiz q(std::stoi(quizIDStr), semesterID, course, syllabus, quizDate);
            if (doneStr == "1")
                q.markDone();

            quizzes.push_back(q);
        }
        return quizzes;
    }

public:
    string get_warningCourse()
    {

        return warning_course;
    }
    double getfilluptime()
    {
        return fillup_time;
    }
    int getRemainingDays()
    {
        return remainingDay;
    }

    static CourseType findLeastStudied(
        const std::vector<CourseType> &courses,
        const std::map<std::string, double> &studyTime)
    {
        CourseType weakest = courses[0];
        double minTime = getTime(weakest, studyTime);

        for (const auto &c : courses)
        {
            double t = getTime(c, studyTime);
            if (t < minTime)
            {
                minTime = t;
                weakest = c;
            }
        }
        return weakest;
    }

    std::vector<Quiz> getAllQuizzes(const std::vector<Course> &courses, int semID)
    {
        std::vector<Quiz> all;

        for (const auto &course : courses)
        {
            auto quizzes = loadQuizzes(semID, course.getCourseName());
            all.insert(all.end(), quizzes.begin(), quizzes.end());
        }

        return all;
    }

    void generateSuggestions(
        const std::vector<CourseType> &courses,
        int semesterID,
        const std::map<std::string, double> &studyTime)
    {

        auto weakCourse = findLeastStudied(courses, studyTime);
        std::cout << "\n> Suggestion: You should focus more on: " << weakCourse.getCourseName() << "\n";

        Routine<std::pair<CourseType, Quiz>> quizRoutine;

        for (const auto &c : courses)
        {
            std::vector<Quiz> quizzes = loadQuizzes(semesterID, c.getCourseName());
            for (const auto &q : quizzes)
            {
                quizRoutine.addItem({c, q});
            }
        }

        std::cout << "\n---- Upcoming Quizzes (Sorted by Date) ----\n";
        quizRoutine.sortRoutine([](const std::pair<CourseType, Quiz> &a, const std::pair<CourseType, Quiz> &b)
                                {
    tm aCopy = a.second.getQuizDate();
    tm bCopy = b.second.getQuizDate();
    return mktime(&aCopy) < mktime(&bCopy); });

        for (const auto &item : quizRoutine.getSchedule())
        {
            // q.displayQuizInfo();
            const auto &[c, q] = item;
            cout << q << endl; // << operator overloaded for displaying quiz info
            double studied = 0.0;
            auto it = studyTime.find(c.getCourseName());
            if (it != studyTime.end())
                studied = it->second;

            std::cout << "Progress: " << studied << " minutes studied.\n";
            std::cout << "-------------------------------------------\n";
            time_t now = time(nullptr);
            tm date = q.getQuizDate();
            time_t quizTime = mktime(&date);
            double days = difftime(quizTime, now) / (60 * 60 * 24);
            if (studied < 30)
            {
                warning_course = c.getCourseName();

                fillup_time = studied;
                remainingDay = days;
            }
        }
    }
    ~SmartSuggestion()
    {
        cout << "Destructing smartsuggestion for sugg\n";
    }
};
