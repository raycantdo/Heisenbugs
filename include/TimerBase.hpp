#ifndef TIMERBASE_HPP
#define TIMERBASE_HPP

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used Windows headers
//#define NOMINMAX // std::min std::max conflict *note to self: mingw already defined it so no need to redefine :)

#include <iostream>
#include <chrono>
#include <thread>
#include <windows.h>
#include <conio.h>
#include <stdexcept>

class TimerException : public std::runtime_error
{
public:
    TimerException(const std::string& msg) : std::runtime_error(msg) {}
};

class TimerBase
{
public:
    virtual ~TimerBase() {}

    // Pure virtual function
    virtual void start(class User& user, std::string courseName) = 0;

    template <typename T>
    bool countdown(T minutes, std::string label)
    {
        int totalSeconds = static_cast<int>(minutes * 60);
        
        std::cout << std::endl << "--- " << label << " (" << minutes << "m) ---" << std::endl;
        std::cout << "Press [Q] to stop the Pomodoro loop." << std::endl;

        for (int i = totalSeconds; i > 0; --i)
        {
            // q to stop
            if (_kbhit()) //keyboard hit
            {
                char ch = _getch(); // get char from keyboard buffer
                if (ch == 'q' || ch == 'Q')
                {
                    std::cout << "\nSession stopped early." << std::endl;
                    return false; 
                }
            }

            // Update timer display on the same line
            std::cout << "\rRemaining: " << i / 60 << "m " << i % 60 << "s   " << std::flush;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // SESSION FINISHED ALERTS
        std::cout << "\n\n*** " << label << " FINISHED! ***" << std::endl;

        // red alert
        system("color 4F"); //4 = Red Background, F = White Text

        // beep
        MessageBeep(MB_ICONINFORMATION); 

        // pause 1 sec
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
        
        // reset color
        system("color 0F"); 

        return true;
    }
};

#endif