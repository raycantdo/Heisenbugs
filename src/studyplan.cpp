#include "StudyPlan.h"

void printMenu() 
{
    cout << "\n=== Study Plan Menu ===" << endl;
    cout << "1. Display Summary" << endl;
    cout << "2. List All Courses" << endl;
    cout << "3. List All Topics" << endl;
    cout << "4. Add Course" << endl;
    cout << "5. Add Topic" << endl;
    cout << "6. Schedule Topic" << endl;
    cout << "7. Mark Topic Complete" << endl;
    cout << "8. Show Course Progress" << endl;
    cout << "9. Save Plan to File" << endl;
    cout << "10. Load Plan from File" << endl;
    cout << "0. Exit" << endl;
    cout << "Choice: ";
}

int main() 
{
    try 
    {
        cout << "\n=== Study Plan Management System ===\n";
        
        // Clear input buffer
        cin.sync();
        
        // Create a study plan
        StudyPlan myPlan("Semester 1 Plan", "John Doe", 3);
        
        // Add some initial courses
        myPlan.addCourse("C++ Programming");
        myPlan.addCourse("Data Structures");
        myPlan.addCourse("Mathematics");
        
        // Add some initial topics
        myPlan.addTopic("C++ Programming", "Classes and Objects", 4);
        myPlan.addTopic("C++ Programming", "Inheritance", 3);
        myPlan.addTopic("C++ Programming", "Polymorphism", 3);
        myPlan.addTopic("Data Structures", "Linked Lists", 5);
        myPlan.addTopic("Data Structures", "Stacks and Queues", 4);
        myPlan.addTopic("Mathematics", "Calculus Review", 2);
        myPlan.addTopic("Mathematics", "Linear Algebra", 4);
        
        // Schedule some topics
        myPlan.scheduleTopic(0, Date(5, 9, 2024));
        myPlan.scheduleTopic(1, Date(7, 9, 2024));
        myPlan.scheduleTopic(3, Date(6, 9, 2024));
        
        // Mark some progress
        myPlan.markTopicComplete(0, 4);
        myPlan.markTopicInProgress(3);
        
        // Display initial summary
        myPlan.displaySummary();
        myPlan.listCourses();
        myPlan.listTopics();
        
        // Save to file demonstration
        cout << "\nSaving plan to file..." << endl;
        myPlan.saveToFile("myplan.txt");
        
        // Load into a new plan to verify
        cout << "\nLoading plan from file..." << endl;
        StudyPlan loadedPlan;
        loadedPlan.loadFromFile("myplan.txt");
        loadedPlan.displaySummary();
        loadedPlan.listTopicsByCourse("C++ Programming");
        
        cout << "\nC++ Progress: " 
             << loadedPlan.getCourseProgress("C++ Programming") << "%" << endl;
        
    } catch (const exception& e) 
    {
        cerr << "Error: " << e.what() << endl;
    }
    
    cout << "\nPress Enter to exit...";
    cin.get();
}