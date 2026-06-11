// SoundPlayer.h - singleton that manages a small pool of sf::Sound voices.
// Allows playing multiple overlapping sounds (rapid-fire shots, simultaneous
// explosions) by rotating through the pool.

#pragma once

#include <SFML/Audio.hpp>
#include <vector>
#include <optional>
#include <string>

class SoundPlayer
{
private:
    // Pool of sf::Sound voices. We rotate through these so multiple sounds
    // can play at once. 8 voices is plenty for an arcade game.
    // std::optional because sf::Sound needs a buffer in its constructor.
    static const int POOL_SIZE = 8;
    std::vector<std::optional<sf::Sound>> pool;

    // Index of the next voice to use (round-robin).
    int nextVoice;

    // Master volume 0..100.
    float volume;

    // Background music. Unlike sounds, this is streamed from disk (large file).
    // SFML 3.x: sf::Music has no default constructor, hence optional.
    std::optional<sf::Music> music;

    // Music volume (separate from SFX so they can be balanced independently).
    float musicVolume;

    SoundPlayer();

public:
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;

    static SoundPlayer& get();

    // Play a sound by its AssetManager-registered name.
    // Safe to call multiple times rapidly - pool rotates automatically.
    void play(const std::string& soundName);

    // Set master volume (0 = silent, 100 = full).
    void setVolume(float v);

    // Load and start looping background music from a file path.
    // Pass an empty path to stop the music.
    void playMusic(const std::string& path);
    void stopMusic();
    void setMusicVolume(float v);
};