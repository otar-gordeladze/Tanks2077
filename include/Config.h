// Config.h - singleton class that loads game settings from a text file.
// Allows tuning the game without recompiling - just edit config/settings.txt.

#pragma once

#include <string>
#include <unordered_map>

class Config
{
private:
    // Storage for parsed key-value pairs (everything stored as string).
    std::unordered_map<std::string, std::string> values;

    // Private constructor - singletons can't be created with "new" or directly.
    Config() = default;

public:
    // Delete copy and assignment - there's only ever ONE Config instance.
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    // Access the single instance. Constructed on first call ("Meyers' singleton").
    static Config& get();

    // Parse a settings file. Returns true on success.
    // Format: key = value, one per line. Lines starting with # are comments.
    bool loadFromFile(const std::string& path);

    // Get a setting with a fallback if not found.
    // The fallback prevents the game from crashing if a setting is missing.
    float       getFloat(const std::string& key, float fallback) const;
    int         getInt(const std::string& key, int fallback) const;
    std::string getString(const std::string& key, const std::string& fallback) const;
};