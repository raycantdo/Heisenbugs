#ifndef POMODOROTIMER_HPP
#define POMODOROTIMER_HPP

#include "TimerBase.hpp"
#include "User.hpp"
#include "StudySession.hpp"
#include "DataHandler.hpp"

class PomodoroTimer : public TimerBase
{
private:
    double workTime;
    double breakTime;
    int sessionCount;

public:
    PomodoroTimer() : workTime(0), breakTime(0), sessionCount(0) {}

    // Operator Overloading
    PomodoroTimer& operator++()
    {
        sessionCount++;
        return *this;
    }

    // Abstract function implementation from TimerBase
    void start(User& user, std::string courseName) override
    {
        try
        {
            std::cout << "\n--- Pomodoro Setup for " << courseName << " ---" << std::endl;
            std::cout << "Enter Study Minutes: ";
            if (!(std::cin >> workTime) || workTime <= 0) throw TimerException("Invalid study time!");
            
            std::cout << "Enter Break Minutes: ";
            if (!(std::cin >> breakTime) || breakTime < 0) throw TimerException("Invalid break time!");

            std::cout << endl << "Starting Pomodoro Timer..." << std::endl;
            std::cout << "Session Length: " << workTime << " minutes, Break Length: " << breakTime << " minutes." << std::endl;
            while (true) {
                // Study Session
                if (!countdown(workTime, "STUDY SESSION")) break;

                // Update static maps from StudySession
                StudySession::courseTotalTime[courseName] += workTime;

                saveData(user); 

                ++(*this); // overloaded operator

                std::cout << "\nTotal Sessions Completed: " << sessionCount << std::endl;

                // Break Session
                if (!countdown(breakTime, "BREAK TIME")) break;
                
                std::cout << endl << "Looping: Next session starting now..." << std::endl;
            }
            std::cout << "\nPomodoro stopped. " << sessionCount << " sessions logged to database." << std::endl;
        }
        catch (const TimerException& e)
        {
            std::cerr << "Timer Error: " << e.what() << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }
    }
};

#endif