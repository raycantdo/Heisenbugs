#ifndef STUDYPLAN_HPP
#define STUDYPLAN_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <ctime>

using namespace std;

// Simple Date class
class Date {
private:
    int day, month, year;
public:
    // Default constructor - current date
    Date() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        day = ltm->tm_mday;
        month = ltm->tm_mon + 1;
        year = ltm->tm_year + 1900;
    }
    
    // Constructor with integers
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    // Constructor with string (format: YYYY-MM-DD)
    Date(const string& dateStr) {
        year = stoi(dateStr.substr(0, 4));
        month = stoi(dateStr.substr(5, 2));
        day = stoi(dateStr.substr(8, 2));
    }
    
    // Getters
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    
    string toString() const {
        return to_string(year) + "-" + 
               (month < 10 ? "0" : "") + to_string(month) + "-" +
               (day < 10 ? "0" : "") + to_string(day);
    }
    
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
};

// Topic class
class Topic {
private:
    string name;
    string courseName;
    int estimatedHours;
    int actualHours;
    string status; // "Planned", "In Progress", "Completed"
    Date scheduledDate;
    
public:
    Topic() : name(""), courseName(""), estimatedHours(0), 
          actualHours(0), status("Planned"),
          scheduledDate(1, 1, 1900) {}
    
    Topic(string n, string course, int hours) 
    : name(n), courseName(course), estimatedHours(hours), 
      actualHours(0), status("Planned"),
      scheduledDate(1, 1, 1900) {}
    
    // Getters
    string getName() const { return name; }
    string getCourseName() const { return courseName; }
    int getEstimatedHours() const { return estimatedHours; }
    int getActualHours() const { return actualHours; }
    string getStatus() const { return status; }
    Date getScheduledDate() const { return scheduledDate; }
    
    // Setters
    void setStatus(string s) { status = s; }
    void setActualHours(int hours) { actualHours = hours; }
    void setScheduledDate(Date d) { scheduledDate = d; }
    
    void markComplete(int hours) {
        status = "Completed";
        actualHours = hours;
    }
};

// StudyPlan class
class StudyPlan {
private:
    string planName;
    string owner;
    Date startDate;
    Date endDate;
    int dailyTarget;
    
    vector<string> courses;
    vector<Topic> topics;
    vector<vector<int>> courseTopics; // Index mapping
    
public:
    // Constructor
    StudyPlan(string name = "My Plan", string student = "Student", int target = 2) 
        : planName(name), owner(student), dailyTarget(target) {
        startDate = Date();
        // Set end date to 30 days after start date
        // For simplicity, just add 1 to month (this is simplified - won't handle year rollover)
        endDate = Date(startDate.getDay(), startDate.getMonth() + 1, startDate.getYear());
    }
    
    // Basic getters
    string getPlanName() const { return planName; }
    string getOwner() const { return owner; }
    Date getStartDate() const { return startDate; }
    Date getEndDate() const { return endDate; }
    int getDailyTarget() const { return dailyTarget; }
    
    // Course management
    void addCourse(string courseName) {
        courses.push_back(courseName);
        courseTopics.push_back(vector<int>());
    }
    
    void removeCourse(int index) {
        if (index >= 0 && index < courses.size()) {
            courses.erase(courses.begin() + index);
            courseTopics.erase(courseTopics.begin() + index);
        }
    }
    
    void listCourses() const {
        cout << "\nCourses:" << endl;
        for (size_t i = 0; i < courses.size(); i++) {
            cout << i + 1 << ". " << courses[i] << " (" 
                 << courseTopics[i].size() << " topics)" << endl;
        }
    }
    
    // Topic management
    void addTopic(string courseName, string topicName, int hours) {
        // Find course index
        for (size_t i = 0; i < courses.size(); i++) {
            if (courses[i] == courseName) {
                topics.push_back(Topic(topicName, courseName, hours));
                courseTopics[i].push_back(topics.size() - 1);
                return;
            }
        }
        throw runtime_error("Course not found: " + courseName);
    }
    
    void removeTopic(int topicIndex) {
        if (topicIndex >= 0 && topicIndex < topics.size()) {
            // Remove from course mapping
            for (auto& ct : courseTopics) {
                for (size_t j = 0; j < ct.size(); j++) {
                    if (ct[j] == topicIndex) {
                        ct.erase(ct.begin() + j);
                        break;
                    }
                }
            }
            topics.erase(topics.begin() + topicIndex);
        }
    }
    
    void listTopics() const {
        if (topics.empty()) {
            cout << "\nNo topics added yet." << endl;
            return;
        }
        
        cout << "\nAll Topics:" << endl;
        for (size_t i = 0; i < topics.size(); i++) {
            cout << i + 1 << ". " << topics[i].getName() 
                 << " (" << topics[i].getCourseName() << ")"
                 << " - " << topics[i].getStatus() 
                 << " - " << topics[i].getEstimatedHours() << " hrs";
            
            if (topics[i].getScheduledDate().toString() != "1900-01-01") {
                cout << " - Scheduled: " << topics[i].getScheduledDate().toString();
            }
            cout << endl;
        }
    }
    
    void listTopicsByCourse(string courseName) const {
        cout << "\nTopics for " << courseName << ":" << endl;
        bool found = false;
        for (const auto& topic : topics) {
            if (topic.getCourseName() == courseName) {
                cout << "  • " << topic.getName() 
                     << " (" << topic.getEstimatedHours() << " hrs)"
                     << " - " << topic.getStatus() << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "  No topics found for this course." << endl;
        }
    }
    
    // Scheduling
    void scheduleTopic(int topicIndex, const Date& date) {
        if (topicIndex >= 0 && topicIndex < topics.size()) {
            topics[topicIndex].setScheduledDate(date);
            cout << "Scheduled: " << topics[topicIndex].getName() 
                 << " on " << date.toString() << endl;
        } else {
            throw runtime_error("Invalid topic index!");
        }
    }
    
    // Progress tracking
    void markTopicComplete(int topicIndex, int hoursSpent) {
        if (topicIndex >= 0 && topicIndex < topics.size()) {
            topics[topicIndex].markComplete(hoursSpent);
            cout << "Marked complete: " << topics[topicIndex].getName() << endl;
        } else {
            throw runtime_error("Invalid topic index!");
        }
    }
    
    void markTopicInProgress(int topicIndex) {
        if (topicIndex >= 0 && topicIndex < topics.size()) {
            topics[topicIndex].setStatus("In Progress");
            cout << "Marked in progress: " << topics[topicIndex].getName() << endl;
        }
    }
    
    float getOverallProgress() const {
        if (topics.empty()) return 0;
        
        int completed = 0;
        for (const auto& topic : topics) {
            if (topic.getStatus() == "Completed") completed++;
        }
        return (completed * 100.0) / topics.size();
    }
    
    float getCourseProgress(string courseName) const {
        int total = 0, completed = 0;
        for (const auto& topic : topics) {
            if (topic.getCourseName() == courseName) {
                total++;
                if (topic.getStatus() == "Completed") completed++;
            }
        }
        if (total == 0) return 0;
        return (completed * 100.0) / total;
    }
    
    int getTotalStudyHours() const {
        int total = 0;
        for (const auto& topic : topics) {
            total += topic.getActualHours();
        }
        return total;
    }
    
    int getRemainingHours() const {
        int remaining = 0;
        for (const auto& topic : topics) {
            if (topic.getStatus() != "Completed") {
                remaining += topic.getEstimatedHours();
            }
        }
        return remaining;
    }
    
    // File I/O
    void saveToFile(string filename) const {
        ofstream file(filename);
        if (!file) throw runtime_error("Cannot open file for writing: " + filename);
        
        file << planName << endl;
        file << owner << endl;
        file << dailyTarget << endl;
        file << startDate.toString() << endl;
        file << endDate.toString() << endl;
        
        file << courses.size() << endl;
        for (const auto& course : courses) {
            file << course << endl;
        }
        
        file << topics.size() << endl;
        for (const auto& topic : topics) {
            file << topic.getName() << endl;
            file << topic.getCourseName() << endl;
            file << topic.getEstimatedHours() << endl;
            file << topic.getActualHours() << endl;
            file << topic.getStatus() << endl;
            file << topic.getScheduledDate().toString() << endl;
        }
        
        file.close();
        cout << "Plan saved to " << filename << endl;
    }
    
    void loadFromFile(string filename) {
        ifstream file(filename);
        if (!file) throw runtime_error("Cannot open file for reading: " + filename);
        
        getline(file, planName);
        getline(file, owner);
        file >> dailyTarget;
        
        string dateStr;
        getline(file, dateStr); // consume newline
        getline(file, dateStr);
        startDate = Date(dateStr);
        getline(file, dateStr);
        endDate = Date(dateStr);
        
        int numCourses;
        file >> numCourses;
        file.ignore();
        
        courses.clear();
        courseTopics.clear();
        for (int i = 0; i < numCourses; i++) {
            string course;
            getline(file, course);
            courses.push_back(course);
            courseTopics.push_back(vector<int>());
        }
        
        int numTopics;
        file >> numTopics;
        file.ignore();
        
        topics.clear();
        for (int i = 0; i < numTopics; i++) {
            string name, courseName, status, scheduledStr;
            int estHours, actHours;
            
            getline(file, name);
            getline(file, courseName);
            file >> estHours >> actHours;
            file.ignore();
            getline(file, status);
            getline(file, scheduledStr);
            
            Topic topic(name, courseName, estHours);
            topic.setActualHours(actHours);
            topic.setStatus(status);
            
            if (scheduledStr != "1900-01-01") {
                topic.setScheduledDate(Date(scheduledStr));
            }
            
            topics.push_back(topic);
            
            // Update courseTopics mapping
            for (size_t j = 0; j < courses.size(); j++) {
                if (courses[j] == courseName) {
                    courseTopics[j].push_back(i);
                    break;
                }
            }
        }
        
        file.close();
        cout << "Plan loaded from " << filename << endl;
    }
    
    // Display summary
    void displaySummary() const {
        cout << "\n" << string(50, '=') << endl;
        cout << "STUDY PLAN: " << planName << endl;
        cout << string(50, '=') << endl;
        cout << "Owner: " << owner << endl;
        cout << "Duration: " << startDate.toString() << " to " << endDate.toString() << endl;
        cout << "Daily Target: " << dailyTarget << " hours" << endl;
        cout << "Total Courses: " << courses.size() << endl;
        cout << "Total Topics: " << topics.size() << endl;
        cout << "Overall Progress: " << getOverallProgress() << "%" << endl;
        cout << "Hours Completed: " << getTotalStudyHours() << endl;
        cout << "Hours Remaining: " << getRemainingHours() << endl;
        cout << string(50, '=') << endl;
    }
};

#endif