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
#include <fstream>    // ← NEW: for save/load
#include <sstream>    // ← NEW: for save/load

namespace StudyTracker {

// Custom Exception
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

    // ── FIX 1: C++17-compatible isSameDay using time_t instead of chrono::days
    bool isSameDay(const std::chrono::system_clock::time_point& date1,
                   const std::chrono::system_clock::time_point& date2) const {
        time_t t1 = std::chrono::system_clock::to_time_t(date1);
        time_t t2 = std::chrono::system_clock::to_time_t(date2);
        tm tm1 = *localtime(&t1);
        tm tm2 = *localtime(&t2);
        return (tm1.tm_year == tm2.tm_year &&
                tm1.tm_mon  == tm2.tm_mon  &&
                tm1.tm_mday == tm2.tm_mday);
    }

public:
    // Rule of Five
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

    // Core Methods
    void addCourse(const std::string& courseName, const T& data = T()) {
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
                return; // Same day, don't increment
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

    // Getters
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

    // Operator Overloads
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

    using iterator       = typename std::vector<Course>::iterator;
    using const_iterator = typename std::vector<Course>::const_iterator;

    iterator       begin()       { return courses.begin(); }
    iterator       end()         { return courses.end(); }
    const_iterator begin() const { return courses.begin(); }
    const_iterator end()   const { return courses.end(); }

    size_t size()  const { return courses.size(); }
    bool   empty() const { return courses.empty(); }

    std::vector<std::string> getCourseNames() const {
        std::vector<std::string> names;
        names.reserve(courses.size());
        for (const auto& course : courses)
            names.push_back(course.name);
        return names;
    }

    void clear() {
        courses.clear();
        additionalData.clear();
    }

    static size_t getInstanceCount() { return instanceCount; }

    // ── FIX 2: saveToFile / loadFromFile for streak persistence ──────────────
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) throw StreakException("Cannot open file for writing: " + filename);

        file << courses.size() << "\n";
        for (size_t i = 0; i < courses.size(); ++i) {
            const auto& c = courses[i];
            // Convert time_point to time_t for portable storage
            time_t t = std::chrono::system_clock::to_time_t(c.lastStudyDate);
            file << c.name          << "\n";
            file << c.currentStreak << "\n";
            file << c.bestStreak    << "\n";
            file << t               << "\n";
            // additionalData: use streaming operator (works for string, int, float, etc.)
            std::ostringstream oss;
            oss << additionalData[i];
            file << oss.str()       << "\n";
        }
        file.close();
        std::cout << "Streaks saved to " << filename << std::endl;
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw StreakException("Cannot open file for reading: " + filename);

        courses.clear();
        additionalData.clear();

        size_t numCourses;
        file >> numCourses;
        file.ignore(1000, '\n');

        for (size_t i = 0; i < numCourses; ++i) {
            std::string name, dataStr;
            int currentStreak, bestStreak;
            time_t t;

            std::getline(file, name);
            file >> currentStreak >> bestStreak >> t;
            file.ignore(1000, '\n');
            std::getline(file, dataStr);

            Course c(name);
            c.currentStreak  = currentStreak;
            c.bestStreak     = bestStreak;
            c.lastStudyDate  = std::chrono::system_clock::from_time_t(t);

            courses.push_back(c);

            // Re-construct T from the saved string
            std::istringstream iss(dataStr);
            T data;
            iss >> data;
            additionalData.push_back(data);
        }

        file.close();
        std::cout << "Streaks loaded from " << filename << std::endl;
    }
    // ─────────────────────────────────────────────────────────────────────────

    // Display
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
                if (i < additionalData.size())
                    std::cout << additionalData[i];
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
            if (i < tracker.additionalData.size())
                os << tracker.additionalData[i];
            os << "\n";
        }
    }
    os << std::string(50, '=') << "\n";
    return os;
}

} // namespace StudyTracker

#endif // STREAKTRACKER_HPP