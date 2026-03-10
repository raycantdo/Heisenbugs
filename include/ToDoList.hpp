#ifndef TODOLIST_HPP
#define TODOLIST_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <fstream> 
#include <sstream>  

using namespace std;

class ToDoList {

private:
    enum Priority { LOW = 1, MEDIUM = 2, HIGH = 3 };

    struct Task {
        int id;
        string title;
        string subject;
        string category;
        Priority priority;
        time_t deadline;
        bool completed;
        double study_hours;
        time_t created_time;
        time_t completed_time;

        Task(int i, string t, string s, string c)
            : id(i), title(t), subject(s), category(c),
              priority(MEDIUM), deadline(0),
              completed(false), study_hours(0),
              created_time(time(0)), completed_time(0) {}
              
        // Default constructor for loading
        Task() : id(0), title(""), subject(""), category(""),
                priority(MEDIUM), deadline(0),
                completed(false), study_hours(0),
                created_time(0), completed_time(0) {}
    };

    vector<Task> tasks;
    string owner;
    time_t created_date;
    time_t last_modified;
    int next_id;

    static inline int total_lists_created = 0;

    // Helper: update modification time
    void updateModifiedTime() {
        last_modified = time(0);
    }

    // Helper: find task index
    int findTaskIndex(int task_id) const {
        for (size_t i = 0; i < tasks.size(); i++)
            if (tasks[i].id == task_id)
                return i;
        return -1;
    }

    // Helper: priority ranking for sorting
    static int priorityRank(Priority p) {
        return static_cast<int>(p);
    }

    // Helper: convert time_t to string for file storage
    string timeToString(time_t t) const {
        if (t == 0) return "0";
        return to_string(t);
    }
    
    // Helper: convert string to time_t for file loading
    time_t stringToTime(const string& str) const {
        if (str == "0") return 0;
        return static_cast<time_t>(stoll(str));
    }

public:
    // Constructor
    ToDoList(string studentName = "Student")
        : owner(studentName), next_id(1) {
        created_date = time(0);
        last_modified = created_date;
        total_lists_created++;
    }

    // Display tasks
    void displayTasks() const {
        if (tasks.empty()) {
            cout << "\n   [!] No tasks in your list yet." << endl;
            return;
        }

        cout << "\n==================== YOUR TO-DO LIST ====================" << endl;
        
        cout << left 
             << setw(5)  << "ID" 
             << setw(18) << "TITLE" 
             << setw(15) << "SUBJECT" 
             << setw(10) << "PRIORITY"
             << "STATUS" << endl;
                
        cout << "---------------------------------------------------------" << endl;

        for (const auto& t : tasks) {
            string priorityStr = (t.priority == HIGH) ? "HIGH" : 
                                 (t.priority == MEDIUM) ? "MED" : "LOW";
            
            cout << left 
                 << setw(5)  << t.id 
                 << setw(18) << (t.title.length() > 15 ? t.title.substr(0, 15) + "..." : t.title)
                 << setw(15) << t.subject
                 << setw(10) << priorityStr
                 << (t.completed ? "[X]" : "[ ]") 
                 << endl;
        }
        cout << "=========================================================\n" << endl;
    }

    // Static getter
    static int getTotalListsCreated() {
        return total_lists_created;
    }

    // Add task to the list
    void add_task(string title, string subject, string category) {
        tasks.emplace_back(next_id++, title, subject, category);
        updateModifiedTime();
    }

    // Remove task from the list
    void remove_task(int task_id) {
        int index = findTaskIndex(task_id);
        if (index != -1) {
            tasks.erase(tasks.begin() + index);
            updateModifiedTime();
        }
    }

    void clear_tasks() {
        tasks.clear();
        updateModifiedTime();
    }

    // Update task title
    void update_task(int task_id, string new_title) {
        int index = findTaskIndex(task_id);
        if (index != -1) {
            tasks[index].title = new_title;
            updateModifiedTime();
        }
    }

    // Mark a task as completed
    void mark_complete(int task_id) {
        int index = findTaskIndex(task_id);
        if (index != -1) {
            tasks[index].completed = true;
            tasks[index].completed_time = time(0);
            updateModifiedTime();
        }
    }

    // Mark a task as incomplete
    void mark_incomplete(int task_id) {
        int index = findTaskIndex(task_id);
        if (index != -1) {
            tasks[index].completed = false;
            tasks[index].completed_time = 0;
            updateModifiedTime();
        }
    }

    // Calculate the completion rate
    double calculate_completion_rate() const {
        if (tasks.empty()) return 0.0;

        int completed = 0;
        for (const auto &t : tasks)
            if (t.completed) completed++;

        return (double)completed / tasks.size() * 100.0;
    }

    // Set the priority of a task
    void set_priority(int task_id, int level) {
        int index = findTaskIndex(task_id);
        if (index != -1) {
            tasks[index].priority = static_cast<Priority>(level);
            updateModifiedTime();
        }
    }

    // Sort tasks by priority
    void sort_by_priority() {
        sort(tasks.begin(), tasks.end(),
             [](const Task &a, const Task &b) {
                 return priorityRank(a.priority) >
                        priorityRank(b.priority);
             });
    }

    // Sort tasks by deadline
    void sort_by_deadline() {
        sort(tasks.begin(), tasks.end(),
             [](const Task &a, const Task &b) {
                 return a.deadline < b.deadline;
             });
    }

    // Sort tasks by subject
    void sort_by_subject() {
        sort(tasks.begin(), tasks.end(),
             [](const Task &a, const Task &b) {
                 return a.subject < b.subject;
             });
    }

    // Add study time to a task
    void add_study_time(int task_id, double hours) {
        int index = findTaskIndex(task_id);
        if (index != -1 && hours > 0) {
            tasks[index].study_hours += hours;
            updateModifiedTime();
        }
    }

    // Get total study time for all tasks
    double get_total_study_time() const {
        double total = 0;
        for (const auto &t : tasks)
            total += t.study_hours;
        return total;
    }

    // Add deadline to a task
    void add_deadline(int task_id, time_t deadline_time) {
        int index = findTaskIndex(task_id);
        if (index != -1) {
            tasks[index].deadline = deadline_time;
            updateModifiedTime();
        }
    }

    // Get overdue tasks
    vector<int> get_overdue_tasks() const {
        vector<int> overdue_ids;
        time_t now = time(0);

        for (const auto &t : tasks) {
            if (!t.completed && t.deadline != 0 && t.deadline < now)
                overdue_ids.push_back(t.id);
        }
        return overdue_ids;
    }

    // Generate progress report
    void generate_progress_report() const {
        displayTasks();
        cout << "\nOwner: " << owner << endl;
        cout << "Total Tasks: " << tasks.size() << endl;
        cout << "Completion Rate: "
             << calculate_completion_rate() << "%\n";
        cout << "Total Study Hours: "
             << get_total_study_time() << " hrs\n";
    }

    // Save to-do list to file
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Cannot open file for writing: " + filename);
        }

        // Write header and metadata
        file << "TODO_LIST_V1" << endl;
        file << owner << endl;
        file << created_date << endl;
        file << last_modified << endl;
        file << next_id << endl;

        // Write number of tasks
        file << tasks.size() << endl;

        // Write each task
        for (const auto& task : tasks) {
            file << task.id << endl;
            file << task.title << endl;
            file << task.subject << endl;
            file << task.category << endl;
            file << static_cast<int>(task.priority) << endl;
            file << task.deadline << endl;
            file << task.completed << endl;
            file << task.study_hours << endl;
            file << task.created_time << endl;
            file << task.completed_time << endl;
        }

        file.close();
        cout << "✅ To-Do list saved to " << filename << endl;
    }

    // Load to-do list from file
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Cannot open file for reading: " + filename);
        }

        // Clear existing tasks
        tasks.clear();

        // Check version
        string version;
        getline(file, version);
        if (version != "TODO_LIST_V1") {
            throw runtime_error("Unsupported file version!");
        }

        // Read metadata
        getline(file, owner);
        file >> created_date;
        file >> last_modified;
        file >> next_id;

        // Read number of tasks
        size_t taskCount;
        file >> taskCount;
        file.ignore(); // Consume newline

        // Read each task
        for (size_t i = 0; i < taskCount; i++) {
            Task task;
            
            file >> task.id;
            file.ignore();
            
            getline(file, task.title);
            getline(file, task.subject);
            getline(file, task.category);
            
            int priorityInt;
            file >> priorityInt;
            task.priority = static_cast<Priority>(priorityInt);
            
            file >> task.deadline;
            file >> task.completed;
            file >> task.study_hours;
            file >> task.created_time;
            file >> task.completed_time;
            file.ignore();

            tasks.push_back(task);
        }

        file.close();
        cout << "✅ To-Do list loaded from " << filename << endl;
    }

    // Save in simple text format (like your team's quiz file)
    void saveToSimpleFormat(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Cannot open file for writing: " + filename);
        }

        for (const auto& task : tasks) {
            // Format: id|priority|subject|title|deadline|study_hours|completed
            file << task.id << "|"
                 << static_cast<int>(task.priority) << "|"
                 << task.subject << "|"
                 << task.title << "|"
                 << task.deadline << "|"
                 << task.study_hours << "|"
                 << task.completed << endl;
        }

        file.close();
        cout << "✅ To-Do list saved to " << filename << " (simple format)" << endl;
    }

    // Load from simple text format
    void loadFromSimpleFormat(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Cannot open file for reading: " + filename);
        }

        tasks.clear();
        string line;
        
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            Task task;
            
            // Parse pipe-separated format: id|priority|subject|title|deadline|study_hours|completed
            getline(ss, token, '|');
            task.id = stoi(token);
            
            getline(ss, token, '|');
            task.priority = static_cast<Priority>(stoi(token));
            
            getline(ss, task.subject, '|');
            getline(ss, task.title, '|');
            
            getline(ss, token, '|');
            task.deadline = stoll(token);
            
            getline(ss, token, '|');
            task.study_hours = stod(token);
            
            getline(ss, token, '|');
            task.completed = (token == "1");
            
            task.created_time = time(0); // Set to current time
            task.completed_time = task.completed ? time(0) : 0;
            
            tasks.push_back(task);
        }

        // Update next_id
        if (!tasks.empty()) {
            int maxId = 0;
            for (const auto& t : tasks) {
                if (t.id > maxId) maxId = t.id;
            }
            next_id = maxId + 1;
        }

        file.close();
        cout << "✅ To-Do list loaded from " << filename << " (simple format)" << endl;
        updateModifiedTime();
    }

    // Export as CSV
    void exportToCSV(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Cannot open file for writing: " + filename);
        }

        // Write header
        file << "ID,Title,Subject,Category,Priority,Deadline,Completed,Study Hours\n";

        for (const auto& task : tasks) {
            string priorityStr = (task.priority == HIGH) ? "HIGH" : 
                                 (task.priority == MEDIUM) ? "MEDIUM" : "LOW";
            
            file << task.id << ","
                 << "\"" << task.title << "\","
                 << "\"" << task.subject << "\","
                 << "\"" << task.category << "\","
                 << priorityStr << ","
                 << task.deadline << ","
                 << (task.completed ? "Yes" : "No") << ","
                 << task.study_hours << endl;
        }

        file.close();
        cout << "✅ To-Do list exported to " << filename << " (CSV format)" << endl;
    }
};

#endif