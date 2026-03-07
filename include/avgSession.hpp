#ifndef AVGSESSION_HPP
#define AVGSESSION_HPP
#include "Analytics.hpp"
#include <iostream>
#include "productivityanalyzer.hpp"
class avgSession:public Analytics
{
    public:
    void analyze(const std::vector<StudySession>& sessions) override
    {
        productivityanalyzer p;
        p.analyze(sessions);
        double total=p.getTotalMinutes();
        cout<<"Average session length: "<<total/sessions.size()<<" minutes\n";
    }
};
#endif;