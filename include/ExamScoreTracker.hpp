#ifndef EXAM_SCORE_TRACKER_HPP
#define EXAM_SCORE_TRACKER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

/**
 * Holds exam scores for a single course.
 */
struct ExamScores 
{
    string courseName;
    int midMarks;      
    int midMax;        
    int finalMarks;      
    int finalMax;        

    ExamScores() : midMarks(0), midMax(0), finalMarks(0), finalMax(0) {}
    ExamScores(const string& name) : courseName(name), midMarks(0), midMax(0), finalMarks(0), finalMax(0) {}
};

/**
 *  - Add/update scores for a course.
 *  - Calculate required marks in the remaining exam to reach a target percentage.
 */
class ExamScoreTracker 
{
private:
    map<string, ExamScores> scores;   // key: course name

    // Helper: load from file on construction
    void loadFromFile() {
        ifstream file("exam_scores.txt");
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) 
        {
            // Format: courseName|midMarks|midMax|finalMarks|finalMax
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1+1);
            size_t pos3 = line.find('|', pos2+1);
            size_t pos4 = line.find('|', pos3+1);
            if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos || pos4 == string::npos)
                continue;

            string name = line.substr(0, pos1);
            int midM = stoi(line.substr(pos1+1, pos2-pos1-1));
            int midMax = stoi(line.substr(pos2+1, pos3-pos2-1));
            int finalM = stoi(line.substr(pos3+1, pos4-pos3-1));
            int finalMax = stoi(line.substr(pos4+1));

            ExamScores sc(name);
            sc.midMarks = midM;
            sc.midMax = midMax;
            sc.finalMarks = finalM;
            sc.finalMax = finalMax;
            scores[name] = sc;
        }
        file.close();
    }

public:
    ExamScoreTracker() {
        loadFromFile();
    }

    /**
     * Saves all scores to a text file (exam_scores.txt).
     */
    void saveToFile() const 
    {
        ofstream file("exam_scores.txt");
        if (!file.is_open()) 
        {
            cerr << "Warning: Could not open exam_scores.txt for writing.\n";
            return;
        }

        for (const auto& pair : scores) 
        {
            const ExamScores& sc = pair.second;
            file << sc.courseName << "|"
                 << sc.midMarks << "|" << sc.midMax << "|"
                 << sc.finalMarks << "|" << sc.finalMax << "\n";
        }
        file.close();
    }

    /**
     * Adds or updates scores for a course.
     */
    void setScores(const string& courseName, int midMarks, int midMax, int finalMarks, int finalMax) 
    {
        ExamScores sc(courseName);
        sc.midMarks = midMarks;
        sc.midMax = midMax;
        sc.finalMarks = finalMarks;
        sc.finalMax = finalMax;
        scores[courseName] = sc;
        saveToFile();   // persist after each update
    }

   
    ExamScores* getScores(const string& courseName) 
    {
        auto it = scores.find(courseName);
        if (it != scores.end()) return &(it->second);
        return nullptr;
    }

    /**
     * Calculate current percentage (based on marks obtained vs maximum possible).
     * If total maximum is 0, returns 0.
     */
    double getCurrentPercentage(const string& courseName) const 
    {
        auto it = scores.find(courseName);
        if (it == scores.end()) return 0.0;

        const ExamScores& sc = it->second;
        int obtained = sc.midMarks + sc.finalMarks;
        int maxPossible = sc.midMax + sc.finalMax;
        if (maxPossible == 0) return 0.0;
        return (static_cast<double>(obtained) / maxPossible) * 100.0;
    }

    /**
     * Calculate marks needed in the final exam to achieve a target percentage. assuming final exam marks = 0
     */
    int marksNeededInFinal(const string& courseName, double targetPercent) const 
{
        auto it = scores.find(courseName);
        if (it == scores.end()) return -1;

        const ExamScores& sc = it->second;
        // If final already taken, we can't change it; we will inform.
        if (sc.finalMarks != 0 && sc.finalMax != 0) 
        {
            // If final already taken, we can only suggest what would have been needed.
            return -1; // or we could return current score info.
        }

        int totalMax = sc.midMax + sc.finalMax;
        double requiredTotal = (targetPercent / 100.0) * totalMax;
        int needed = static_cast<int>(requiredTotal - sc.midMarks);
        if (needed < 0) needed = 0;
        if (needed > sc.finalMax) return -1; // impossible
        return needed;
    }

    /**
     * Display a summary of scores and required marks for all courses.
     */
    void displaySummary(double targetPercent) const 
    {
        if (scores.empty()) 
        {
            cout << "No exam scores recorded yet.\n";
            return;
        }

        cout << "\n--- Exam Score Tracker (Target: " << targetPercent << "%) ---\n";
        for (const auto& pair : scores) {
            const ExamScores& sc = pair.second;
            double current = (sc.midMarks + sc.finalMarks) / (double)(sc.midMax + sc.finalMax) * 100.0;
            cout << "Course: " << sc.courseName << "\n";
            cout << "   Midterm: " << sc.midMarks << "/" << sc.midMax << "\n";
            cout << "   Final:   " << sc.finalMarks << "/" << sc.finalMax << "\n";
            cout << "   Current: " << current << "%\n";

            if (sc.finalMarks == 0 && sc.finalMax > 0) {
                int needed = marksNeededInFinal(sc.courseName, targetPercent);
                if (needed >= 0) 
                {
                    cout << "   To reach " << targetPercent << "%, you need " << needed
                         << " out of " << sc.finalMax << " in the final.\n";
                } 
                else if (needed == -1) 
                {
                    cout << "   Already above target! (No final needed).\n";
                } 
                else 
                {
                    cout << "   Target not possible (needs more than " << sc.finalMax << ").\n";
                }
            } 
            else 
            {
                cout << "   Final already taken. Overall: " << current << "%\n";
            }
            cout << "\n";
        }
    }

    /**
     * Display only one course's info.
     */

    void displayCourse(const string& courseName, double targetPercent) const {
        auto it = scores.find(courseName);
        if (it == scores.end()) 
        {
            cout << "No scores for " << courseName << ".\n";
            return;
        }
        const ExamScores& sc = it->second;
        double current = (sc.midMarks + sc.finalMarks) / (double)(sc.midMax + sc.finalMax) * 100.0;
        cout << "\n--- " << courseName << " ---\n";
        cout << "Midterm: " << sc.midMarks << "/" << sc.midMax << "\n";
        cout << "Final:   " << sc.finalMarks << "/" << sc.finalMax << "\n";
        cout << "Current: " << current << "%\n";
        if (sc.finalMarks == 0 && sc.finalMax > 0) 
        {
            int needed = marksNeededInFinal(courseName, targetPercent);
            if (needed >= 0) 
            {
                cout << "To reach " << targetPercent << "%, you need " << needed
                     << " out of " << sc.finalMax << " in the final.\n";
            } 
            else if (needed == -1) 
            {
                cout << "Already above target! (No final needed).\n";
            } 
            else 
            {
                cout << "Target not possible (needs more than " << sc.finalMax << ").\n";
            }
        }
    }
};

#endif // EXAM_SCORE_TRACKER_HPP
