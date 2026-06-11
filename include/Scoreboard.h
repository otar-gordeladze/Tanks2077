// Scoreboard.h - manages a top-N high-scores list with file persistence.
// Scores are kept sorted from highest to lowest.

#pragma once

#include <vector>
#include <string>

class Scoreboard
{
private:
    // The list of scores, sorted from highest to lowest.
    std::vector<int> scores;

    // Maximum number of scores to keep.
    static const int MAX_SCORES = 5;

    // File path on disk where scores are saved.
    std::string filePath;

public:
    // Constructor. Default file path is "scoreboard.txt" in the working directory.
    Scoreboard(const std::string& path = "scoreboard.txt");

    // Load scores from file. If the file doesn't exist, start with an empty list.
    void loadFromFile();

    // Save scores to file. Overwrites the file.
    void saveToFile() const;

    // Add a new score. Keeps only the top MAX_SCORES.
    void addScore(int score);

    // Read-only access to the scores.
    const std::vector<int>& getScores() const { return scores; }

    // Returns true if the given score qualifies for the top list.
    bool isHighScore(int score) const;
};