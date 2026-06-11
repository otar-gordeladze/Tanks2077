// Scoreboard.cpp - implementation of the persistent top-scores list.

#include "Scoreboard.h"
#include <fstream>
#include <algorithm>     // for std::sort

Scoreboard::Scoreboard(const std::string& path)
    : filePath(path)
{
}

void Scoreboard::loadFromFile()
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return;     // No file yet - that's fine, start empty.

    scores.clear();
    int s;
    while (file >> s)
    {
        scores.push_back(s);
    }

    // Sort in descending order in case the file was edited manually.
    std::sort(scores.begin(), scores.end(), std::greater<int>());

    // Trim to MAX_SCORES (in case someone added more).
    if (static_cast<int>(scores.size()) > MAX_SCORES)
        scores.resize(MAX_SCORES);
}

void Scoreboard::saveToFile() const
{
    std::ofstream file(filePath);
    if (!file.is_open()) return;    // Couldn't open file - silently skip.

    for (int s : scores)
        file << s << "\n";
}

void Scoreboard::addScore(int score)
{
    // Insert the new score, then re-sort + trim.
    scores.push_back(score);
    std::sort(scores.begin(), scores.end(), std::greater<int>());

    if (static_cast<int>(scores.size()) > MAX_SCORES)
        scores.resize(MAX_SCORES);

    saveToFile();    // Persist immediately so a crash doesn't lose progress.
}

bool Scoreboard::isHighScore(int score) const
{
    // Always a high score if we don't have a full list yet.
    if (static_cast<int>(scores.size()) < MAX_SCORES)
        return true;

    // Otherwise, it must beat the lowest currently-stored score.
    return score > scores.back();
}