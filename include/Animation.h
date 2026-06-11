// Animation.h - reusable sprite-sheet animation timer.
// Holds an ordered list of textures + frame duration. Each update(dt) ticks
// the elapsed time; getCurrentTexture() returns the right frame for "now".
// When all frames have been shown, isFinished() returns true.

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Animation
{
private:
    // Pointers to textures held by AssetManager. Animation does NOT own them.
    std::vector<sf::Texture*> frames;

    // How long each frame is displayed, in seconds.
    float frameDuration;

    // Time elapsed in the current frame.
    float frameTimer;

    // Index of the currently-displayed frame.
    int currentFrame;

    // If true, animation has played all frames and stopped.
    bool finished;

    // If true, animation loops back to frame 0 after the last frame.
    // For one-shot animations (like explosions), set to false.
    bool looping;

public:
    // Constructor: list of texture names (looked up in AssetManager),
    // frame duration in seconds, and whether to loop.
    Animation(const std::vector<std::string>& textureNames,
              float frameDuration,
              bool looping = false);

    // Advance the animation by dt seconds.
    void update(float dt);

    // Return the texture to draw right now.
    sf::Texture& getCurrentTexture() const;

    // Has the animation reached the end? (Always false for looping animations.)
    bool isFinished() const { return finished; }

    // Restart from the first frame.
    void reset();
};