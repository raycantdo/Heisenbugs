#ifndef COURSE_HPP
#define COURSE_HPP

#include <string>
#include <vector>
#include <iostream>
class Course
{
private:
    std::string courseName;
    float courseCredits;
public:
    Course(std::string name, float credits) : courseName(name), courseCredits(credits) {}


    void setName(const std::string& name)
    {
        courseName = name;
    }
    void setCredits(float credits)
    {
        courseCredits = credits;
    }

    std::string getName() const
    {
        return courseName;
    }
    float getCredits() const
    {
        return courseCredits;
    }

    void displayCourseInfo() const
    {
        std::cout << "Course Name: " << courseName << ", Credits: " << courseCredits << std::endl;
    }
};
#endif