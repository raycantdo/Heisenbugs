#include<string>
#include<iostream>
#include<fstream>
#include <vector>
#include<ctime>
#include<sstream>
#include<map>
using namespace std;
class ClassSession
{
    private:
    string course;
    struct tm dt;
    public:
    ClassSession(string c,int d,int h,int min)
    {
        course=c;
        dt.tm_wday=d;
        dt.tm_hour=h;
        dt.tm_min=min;
    }
    friend ostream& operator<<(ostream& os, const ClassSession& c)
    {
        os <<"\nCourse: " << c.course 
           << "\nDay: " << c.getDayName()
           << "\nTime: " << c.dt.tm_hour<<":"<<c.dt.tm_min<<endl ;
        return os;
    }
    int getday() const
    {
        return dt.tm_wday;
    }
    string getDayName() const 
    {
        static string days[]={"Sunday", "Monday", "Tuesday","Wednesday", "Thursday", "Friday", "Saturday"};
        return days[dt.tm_wday];
    }
};