// Config.cpp - implementation of the Config singleton.

#include "Config.h"
#include <fstream>      // for std::ifstream (file reading)
#include <sstream>      // for std::stringstream (parsing)
#include <iostream>     // for std::cerr (error messages)

// Meyers' singleton: static local variable is constructed on first call,
// destroyed at program exit, and thread-safe by C++11 guarantee.
Config& Config::get()
{
    static Config instance;
    return instance;
}

bool Config::loadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Config: could not open " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        // Skip empty lines and comment lines (start with '#').
        // Find the first non-whitespace character to detect a "#" comment.
        size_t firstNonSpace = line.find_first_not_of(" \t");
        if (firstNonSpace == std::string::npos) continue;       // empty line
        if (line[firstNonSpace] == '#') continue;               // comment

        // Find the '=' separator
        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;               // malformed line

        std::string key   = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);

        // Trim leading/trailing whitespace from both key and value
        auto trim = [](std::string& s) {
            size_t start = s.find_first_not_of(" \t");
            size_t end   = s.find_last_not_of(" \t");
            if (start == std::string::npos) { s.clear(); return; }
            s = s.substr(start, end - start + 1);
        };
        trim(key);
        trim(value);

        if (!key.empty())
            values[key] = value;
    }

    return true;
}

float Config::getFloat(const std::string& key, float fallback) const
{
    auto it = values.find(key);
    if (it == values.end()) return fallback;

    // Try to parse the value as float. If parsing fails, return fallback.
    try
    {
        return std::stof(it->second);
    }
    catch (const std::exception&)
    {
        return fallback;
    }
}

int Config::getInt(const std::string& key, int fallback) const
{
    auto it = values.find(key);
    if (it == values.end()) return fallback;

    try
    {
        return std::stoi(it->second);
    }
    catch (const std::exception&)
    {
        return fallback;
    }
}

std::string Config::getString(const std::string& key, const std::string& fallback) const
{
    auto it = values.find(key);
    if (it == values.end()) return fallback;
    return it->second;
}