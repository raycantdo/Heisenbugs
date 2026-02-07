#ifndef SEMESTER_HPP
#define SEMESTER_HPP

#include <string>
#include <vector>
#include "Course.hpp"

class Semester
{
private:
    int semesterId;
    std::vector<Course> courses;

public:
    Semester(int id) : semesterId(id) {}

    void addCourse(const Course& course)
    {
        courses.push_back(course);
    }

    void displaySemesterInfo() const
    {
        std::cout << "Semester: " << semesterId << std::endl;
        for (const auto& course : courses)
        {
            course.displayCourseInfo();
        }
        std::cout << std::endl;
    }
    int getSemesterId() const {
    return semesterId;
    }

};

#endif