#ifndef PRODUCTIVITYANALYZER_HPP
#define PRODUCTIVITYANALYZER_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include "Analytics.hpp"
#include "StudySession.hpp"
#include "FocusTimeAnalyzer.hpp"

class productivityanalyzer : public Analytics
{
    private:
    double totalMinutes;
public:
    double getTotalMinutes()
    {
        return totalMinutes;
    }

    void analyze(const std::vector<StudySession>& sessions) override
    {
        FocusTimeAnalyzer f;
        f.analyze(sessions);
        string bestLabel=f.getLabel();
         int hourCount[24] = {0};

        totalMinutes = 0;

        for(const auto& s : sessions)
        {
            time_t start=s.getStartTime();
            tm* t=localtime(&start);
            int startHour=t->tm_hour;
            time_t end=s.getEndTime();
            tm* t1=localtime(&end);
            int endHour   = t1->tm_hour;

            for(int h = startHour; h <= endHour; h++)
                hourCount[h]++;

            totalMinutes += s.getDurationInMinutes();
        }

        map<string,int>m;

        for(int h=0; h<24; h++)
        {
            if(h>=5 && h<11)
                m["Morning"] += hourCount[h];
            else if(h>=11 && h<17)
                m["Afternoon"] += hourCount[h];
            else if(h>=17 && h<22)
                m["Evening"] += hourCount[h];
            else
               m[ "Night"] += hourCount[h];
        }
        double productivity=(m[bestLabel]/totalMinutes)*100;
        cout<<"Productivity rate: "<<productivity<<"%\n";
        
    }
};
#endif