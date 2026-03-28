#ifndef FLASHCARD_HPP
#define FLASHCARD_HPP

#include <string>
#include <iostream>
#include <vector>
#include <fstream>   // for file handling (ifstream)
#include <algorithm> // for std::transform (case conversion) and std::random_shuffle
#include <cstdlib>   // for std::rand, std::srand (random number generation)
#include <ctime>     // for std::time (seed for random)
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
    Flashcard(const string &q, const string &a) : question(q), answer(a) {}

    string getQuestion() const { return question; }

    string getAnswer() const { return answer; }

    // Displays  question, waits for the user answer, and checks correctness.

    bool ask() const
    {

        cout << "\n " << question << "\nYour answer (type 'q' to quit): ";

        string userInput;
        getline(cin, userInput);
        if (userInput == "q")
        {
            return false;
        }
        // Create copies for case‑insensitive comparison
        string correct = answer;
        string user = userInput;

        transform(correct.begin(), correct.end(), correct.begin(), ::tolower);
        transform(user.begin(), user.end(), user.begin(), ::tolower);

        if (user == correct)
        {
            cout << "✅ Correct!\n";
        }
        else
        {
            cout << "Incorrect. The correct answer is: " << answer << "\n";
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
     */
    bool loadForCourse(const string &courseName)
    {
        // Build the filename based on the course name
        string filename = "flashcards_" + courseName + ".txt";

        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "No flashcards found for " << courseName << ".\n";
            return false;
        }

        cards.clear();

        string line;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            size_t pos = line.find('|');
            if (pos != string::npos)
            {

                // Split the line into question (before '|') and answer (after '|')
                string q = line.substr(0, pos);
                string a = line.substr(pos + 1);

                cards.push_back(Flashcard(q, a));
            }
            else
            {
                //  line is malformed if doesn't contain '|',
                cerr << "Warning: Skipping malformed line in " << filename << ": " << line << "\n";
            }
        }

        file.close();

        // Report how many cards were loaded
        cout << "Loaded " << cards.size() << " flashcards for " << courseName << ".\n";

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
        // srand(static_cast<unsigned>(time(nullptr)));
        unsigned seed = 0;

        // Shuffle the indices to randomize the order of questions
        // random_shuffle(indices.begin(), indices.end());
        shuffle(indices.begin(), indices.end(), default_random_engine(seed));

        // Start quiz
        cout << "\n=== Flashcard Quiz ===\n";
        cout << "You will be shown " << cards.size() << " cards.\n";

        // Iterate through the shuffled indices
        for (size_t idx : indices)
        {
            // Ask the flashcard at position 'idx'
            bool continueQuiz = cards[idx].ask();
            if (!continueQuiz)
            {
                cout << "\nExiting quiz ...\n";
                break;
            }
            cout << "Press Enter to continue...";
            cin.get(); // Waits for Enter key
        }

        // Quiz finished
        cout << "\nQuiz finished! Great job.\n";
    }
};

#endif
