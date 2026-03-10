// StreakTracker.hpp
#ifndef STREAKTRACKER_HPP
#define STREAKTRACKER_HPP

#include <string>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <iomanip>

namespace StudyTracker {

//Custom Exception
class StreakException : public std::runtime_error {
public:
    explicit StreakException(const std::string& message) 
        : std::runtime_error(message) {}
};

struct Course {
    std::string name;
    int currentStreak;
    int bestStreak;
    std::chrono::system_clock::time_point lastStudyDate;
    
    explicit Course(const std::string& courseName) 
        : name(courseName), currentStreak(0), bestStreak(0),
          lastStudyDate(std::chrono::system_clock::now()) {}
};

template <typename T = std::string>
class StreakTracker {
private:
    std::vector<Course> courses;
    std::vector<T> additionalData;
    static size_t instanceCount;

    size_t findCourseIndex(const std::string& courseName) const {
        auto it = std::find_if(courses.begin(), courses.end(),
            [&courseName](const Course& c) { return c.name == courseName; });
        
        if (it == courses.end()) {
            throw StreakException("Course not found: " + courseName);
        }
        return std::distance(courses.begin(), it);
    }

    bool isSameDay(const std::chrono::system_clock::time_point& date1,
                   const std::chrono::system_clock::time_point& date2) const {
        auto dp1 = std::chrono::floor<std::chrono::days>(date1);
        auto dp2 = std::chrono::floor<std::chrono::days>(date2);
        return dp1 == dp2;
    }

public:
    //Rule of Five
    StreakTracker() { instanceCount++; }
    virtual ~StreakTracker() { instanceCount--; }

    StreakTracker(const StreakTracker& other)
        : courses(other.courses), additionalData(other.additionalData) {
        instanceCount++;
    }

    StreakTracker(StreakTracker&& other) noexcept
        : courses(std::move(other.courses)), 
          additionalData(std::move(other.additionalData)) {
        instanceCount++;
    }

    StreakTracker& operator=(const StreakTracker& other) {
        if (this != &other) {
            courses = other.courses;
            additionalData = other.additionalData;
        }
        return *this;
    }

    StreakTracker& operator=(StreakTracker&& other) noexcept {
        if (this != &other) {
            courses = std::move(other.courses);
            additionalData = std::move(other.additionalData);
        }
        return *this;
    }

    //Core Method
    void addCourse(const std::string& courseName, const T& data = T()) {
        // Check for duplicates
        auto it = std::find_if(courses.begin(), courses.end(),
            [&courseName](const Course& c) { return c.name == courseName; });
        
        if (it != courses.end()) {
            throw StreakException("Course already exists: " + courseName);
        }
        
        courses.emplace_back(courseName);
        additionalData.push_back(data);
    }

    void removeCourse(const std::string& courseName) {
        size_t index = findCourseIndex(courseName);
        courses.erase(courses.begin() + index);
        additionalData.erase(additionalData.begin() + index);
    }

    void recordStudyDay(const std::string& courseName) {
        size_t index = findCourseIndex(courseName);
        auto& course = courses[index];
        auto now = std::chrono::system_clock::now();

        if (course.currentStreak == 0) {
            course.currentStreak = 1;
        } else {
            if (isSameDay(course.lastStudyDate, now)) {
                return; // Same day,don't increment
            }

            auto tomorrow = course.lastStudyDate + std::chrono::hours(24);
            if (now <= tomorrow + std::chrono::hours(12)) { // 12-hour grace period
                course.currentStreak++;
            } else {
                course.currentStreak = 1; // Streak broken
            }
        }

        course.bestStreak = std::max(course.bestStreak, course.currentStreak);
        course.lastStudyDate = now;
    }

    //Getters
    int getCurrentStreak(const std::string& courseName) const {
        return courses[findCourseIndex(courseName)].currentStreak;
    }

    int getBestStreak(const std::string& courseName) const {
        return courses[findCourseIndex(courseName)].bestStreak;
    }

    T getAdditionalData(const std::string& courseName) const {
        return additionalData[findCourseIndex(courseName)];
    }

    void setAdditionalData(const std::string& courseName, const T& data) {
        additionalData[findCourseIndex(courseName)] = data;
    }

    //Operator Overloads
    StreakTracker& operator+=(const StreakTracker& other) {
        for (size_t i = 0; i < other.courses.size(); ++i) {
            const auto& course = other.courses[i];
            
            try {
                addCourse(course.name, other.additionalData[i]);
                size_t idx = findCourseIndex(course.name);
                courses[idx].currentStreak = course.currentStreak;
                courses[idx].bestStreak = course.bestStreak;
                courses[idx].lastStudyDate = course.lastStudyDate;
            } catch (const StreakException&) {
                // Course exists, merge best streak
                size_t idx = findCourseIndex(course.name);
                courses[idx].bestStreak = std::max(courses[idx].bestStreak, course.bestStreak);
            }
        }
        return *this;
    }

    bool operator==(const StreakTracker& other) const {
        if (courses.size() != other.courses.size()) return false;
        
        for (size_t i = 0; i < courses.size(); ++i) {
            if (courses[i].name != other.courses[i].name ||
                courses[i].currentStreak != other.courses[i].currentStreak) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const StreakTracker& other) const {
        return !(*this == other);
    }

    int operator[](const std::string& courseName) const {
        return getCurrentStreak(courseName);
    }

    void operator()(const std::string& courseName) {
        recordStudyDay(courseName);
    }

    using iterator = typename std::vector<Course>::iterator;
    using const_iterator = typename std::vector<Course>::const_iterator;

    iterator begin() { return courses.begin(); }
    iterator end() { return courses.end(); }
    const_iterator begin() const { return courses.begin(); }
    const_iterator end() const { return courses.end(); }

    size_t size() const { return courses.size(); }
    bool empty() const { return courses.empty(); }

    std::vector<std::string> getCourseNames() const {
        std::vector<std::string> names;
        names.reserve(courses.size());
        for (const auto& course : courses) {
            names.push_back(course.name);
        }
        return names;
    }

    void clear() {
        courses.clear();
        additionalData.clear();
    }

    static size_t getInstanceCount() { return instanceCount; }

    //Display
    void display() const {
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "STREAK TRACKER REPORT\n";
        std::cout << std::string(50, '=') << "\n";
        
        if (courses.empty()) {
            std::cout << "No courses tracked yet.\n";
        } else {
            std::cout << std::left << std::setw(25) << "Course Name"
                      << std::setw(15) << "Current Streak"
                      << std::setw(15) << "Best Streak"
                      << "Additional Info\n";
            std::cout << std::string(70, '-') << "\n";
            
            for (size_t i = 0; i < courses.size(); ++i) {
                const auto& course = courses[i];
                std::cout << std::left << std::setw(25) << course.name
                          << std::setw(15) << course.currentStreak
                          << std::setw(15) << course.bestStreak;
                
                if (i < additionalData.size()) {
                    std::cout << additionalData[i];
                }
                std::cout << "\n";
            }
        }
        std::cout << std::string(50, '=') << "\n\n";
    }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const StreakTracker<U>& tracker);
};

template <typename T>
size_t StreakTracker<T>::instanceCount = 0;

template <typename T>
std::ostream& operator<<(std::ostream& os, const StreakTracker<T>& tracker) {
    os << "\n" << std::string(50, '=') << "\n";
    os << "STREAK TRACKER REPORT\n";
    os << std::string(50, '=') << "\n";
    
    if (tracker.courses.empty()) {
        os << "No courses tracked yet.\n";
    } else {
        os << std::left << std::setw(25) << "Course Name"
           << std::setw(15) << "Current Streak"
           << std::setw(15) << "Best Streak"
           << "Additional Info\n";
        os << std::string(70, '-') << "\n";
        
        for (size_t i = 0; i < tracker.courses.size(); ++i) {
            const auto& course = tracker.courses[i];
            os << std::left << std::setw(25) << course.name
               << std::setw(15) << course.currentStreak
               << std::setw(15) << course.bestStreak;
            
            if (i < tracker.additionalData.size()) {
                os << tracker.additionalData[i];
            }
            os << "\n";
        }
    }
    os << std::string(50, '=') << "\n";
    return os;
}

} // namespace StudyTracker

#endif // STREAKTRACKER_HPP