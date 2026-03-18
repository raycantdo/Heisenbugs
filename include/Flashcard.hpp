#ifndef FLASHCARD_HPP
#define FLASHCARD_HPP

#include <string>     
#include <iostream>    
#include <vector>     
#include <fstream>     // for file handling (ifstream)
#include <algorithm>   // for std::transform (case conversion) and std::random_shuffle
#include <cstdlib>     // for std::rand, std::srand (random number generation)
#include <ctime>       // for std::time (seed for random)
#include <random>

using namespace std;   

// ==================== Flashcard Class ====================
/**
 * Represents a single flashcard with a question and its correct answer.
 * Provides a method to ask the question, read the user's input and give feedback (correct/incorrect) with case‑insensitive comparison.
 */
class Flashcard 
{
private:
    string question;  
    string answer;     

public:
    Flashcard(const string& q, const string& a) : question(q), answer(a) {}

    string getQuestion() const { return question; }

    string getAnswer() const { return answer; }

     // Displays the question, waits for the user's answer, and checks correctness.

    void ask() const 
    {
    
        cout << "\n❓ " << question << "\nYour answer: ";

        // Read the user's answer (may contain spaces, so used getline)
        string userInput;
        getline(cin, userInput);

        // Create copies for case‑insensitive comparison
        string correct = answer;   // correct answer from the flashcard
        string user = userInput;   // user's input

        // Convert both strings to lowercase using std::transform ::tolower is the C function to convert a character to lowercase
        transform(correct.begin(), correct.end(), correct.begin(), ::tolower);
        transform(user.begin(), user.end(), user.begin(), ::tolower);

        // Compare the lowercase versions
        if (user == correct) 
        {
            cout << "✅ Correct!\n";
        } else 
        {
            cout << "❌ Incorrect. The correct answer is: " << answer << "\n";
        }
    }
};

// ==================== FlashcardManager Class ====================
class FlashcardManager 
{
private:
    vector<Flashcard> cards;   

public:
    /**
     * Loads flashcards for a given course from a text file.
     * File format: each line should contain a question and answer separated by a '|' character.
     * Example line: "What is a class?|A blueprint for objects"
     * 
     * The file name is constructed as: "flashcards_" + courseName + ".txt"
     * (spaces in the course name are allowed).
     * 
     * @param courseName The name of the course (used to build the filename).
     * @return true if at least one flashcard was loaded, false if file couldn't be opened or no valid cards.
     */
    bool loadForCourse(const string& courseName) 
    {
        // Build the filename based on the course name
        string filename = "flashcards_" + courseName + ".txt";

        // Attempt to open the file
        ifstream file(filename);
        if (!file.is_open()) {
            // File doesn't exist or cannot be opened – inform the user
            cout << "No flashcards found for " << courseName << ".\n";
            return false;
        }

        // Clear any previously loaded cards (in case this manager was used before)
        cards.clear();

        string line;
        // Read the file line by line
        while (getline(file, line)) 
        {
            // Skip empty lines
            if (line.empty()) continue;

            // Find the position of the delimiter '|'
            size_t pos = line.find('|');
            if (pos != string::npos) {
                // Split the line into question (before '|') and answer (after '|')
                string q = line.substr(0, pos);
                string a = line.substr(pos + 1);

                // Create a new Flashcard and add it to the vector
                cards.push_back(Flashcard(q, a));
            } else {
                // If a line doesn't contain '|', it's malformed – print a warning
                cerr << "Warning: Skipping malformed line in " << filename << ": " << line << "\n";
            }
        }

        file.close(); // Done reading

        // Report how many cards were loaded
        cout << "Loaded " << cards.size() << " flashcards for " << courseName << ".\n";

        // Return true only if at least one card was loaded
        return !cards.empty();
    }

    /**
     * Runs an interactive quiz with all loaded flashcards.
     * Flashcards are presented in a random order (shuffled).
     * After each question, the user must press Enter to continue.
     */
    void runQuiz() const 
    {
        // If no cards were loaded, nothing to quiz
        if (cards.empty()) 
        {
            cout << "No flashcards to review.\n";
            return;
        }
        // Create a vector of indices (0, 1, 2, ..., cards.size()-1)
        vector<size_t> indices(cards.size());
        for (size_t i = 0; i < cards.size(); ++i)
        {
            indices[i] = i;
        }

        // Seed the random number generator with the current time
        //srand(static_cast<unsigned>(time(nullptr)));
        unsigned seed=0;

         //Shuffle the indices to randomize the order of questions
       //random_shuffle(indices.begin(), indices.end());
        shuffle(indices.begin(), indices.end(),default_random_engine(seed));

        // Start the quiz
        cout << "\n=== Flashcard Quiz ===\n";
        cout << "You will be shown " << cards.size() << " cards.\n";

        // Iterate through the shuffled indices
        for (size_t idx : indices) {
            // Ask the flashcard at position 'idx'
            cards[idx].ask();

            // Pause until the user presses Enter
            cout << "Press Enter to continue...";
            cin.get(); // Waits for Enter key
        }

        // Quiz finished
        cout << "\nQuiz finished! Great job.\n";
    }
};

#endif // FLASHCARD_HPP
