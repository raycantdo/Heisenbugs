#ifndef TODOLIST_HPP
#define TODOLIST_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>

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

public:
    // Constructor
    ToDoList(string studentName)
        : owner(studentName), next_id(1) {
        created_date = time(0);
        last_modified = created_date;
        total_lists_created++;
    }


    void displayTasks() const {
        if (tasks.empty()) {
            std::cout << "\n   [!] No tasks in your list yet." << std::endl;
            return;
        }

        std::cout << "\n==================== YOUR TO-DO LIST ====================" << std::endl;
        
        std::cout << std::left 
                << std::setw(5)  << "ID" 
                << std::setw(18) << "TITLE" 
                << std::setw(15) << "SUBJECT" 
                << "STATUS" << std::endl;
                
        std::cout << "---------------------------------------------------------" << std::endl;

        for (const auto& t : tasks) {
            std::cout << std::left 
                    << std::setw(5)  << t.id 
                    << std::setw(18) << (t.title.length() > 15 ? t.title.substr(0, 15) + "..." : t.title)
                    << std::setw(15) << t.subject
                    << (t.completed ? "[X]" : "[ ]") 
                    << std::endl;
        }
        std::cout << "=========================================================\n" << std::endl;
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
    void set_priority(int task_id, Priority level) {
        int index = findTaskIndex(task_id);
        if (index != -1) {
            tasks[index].priority = level;
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
};

// Initialize static variable
//int ToDoList::total_lists_created = 0;
//initialized using inline in class definition
#endif
