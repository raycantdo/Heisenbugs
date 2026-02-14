#ifndef SEMESTER_HPP
#define SEMESTER_HPP

#include <string>
#include <vector>
#include "Course.hpp"

using namespace std;

class Semester
{
private:
    int semesterId;
    std::vector<Course> courses;

public:
    Semester(int id) : semesterId(id) {}

    // Non-const version for modification
    vector<Course>& getCourses() { return courses; }
    
    // Const version for read-only access
    const vector<Course>& getCourses() const { return courses; }


    void addCourse(const string& name, float credits = 3.0)
    {
        courses.emplace_back(name, credits);
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