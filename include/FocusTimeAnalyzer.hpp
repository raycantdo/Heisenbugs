#include "Analytics.hpp"
#include <map>
#include <iostream>
#include<string>
#include<iomanip>
using namespace std;
class FocusTimeAnalyzer : public Analytics {
    private:
    string label;
public:
    void analyze(const std::vector<StudySession>& sessions) override
    {
         int hourCount[24] = {0};

       
        for (const auto& s : sessions) {
            time_t start=s.getStartTime();
            tm* t=localtime(&start);
            hourCount[t->tm_hour]++;
        }
        int bestStartHour = -1;
        int maxCount = 0;
        for (int h = 0; h < 24; ++h) {
            if (hourCount[h] > maxCount) {
                maxCount = hourCount[h];
                bestStartHour = h;
            }
        }
        double minDuration = 1e9;
        int bestEndHour = bestStartHour;
        for(const auto& s:sessions)
        {
            time_t start=s.getStartTime();
            tm* t=localtime(&start);
            if(t->tm_hour==bestStartHour)
            {
                double duration=s.getDurationInMinutes();
                if(duration<minDuration)
                {
                    minDuration=duration;
                    time_t end=s.getEndTime();
                    tm* t1=localtime(&end);
                    bestEndHour=t1->tm_hour;
                }
            }
        }
        if(bestStartHour==bestEndHour)
        {
            bestEndHour=bestStartHour+1;
        }
        
        if(bestStartHour>=5 && bestStartHour<=11)
        {
            label="Morning";
        }
        else if(bestStartHour>=12 && bestStartHour<=17)
        {
            label="Afternoon";
        }
        else if(bestStartHour>=18 && bestStartHour<=23)
        {
            label="Evening";
        }
        else
            label="Night";
        

        cout<<"Most focused time: " <<label<<" "<<setfill('0')<<setw(2)<< bestStartHour<<":00 - "<<setfill('0')<<setw(2)<<bestEndHour<<":00\n";
    }
    string getLabel()
    {
        return label;
    }
};