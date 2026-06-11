// Animation.cpp - implementation of the reusable frame animator.

#include "Animation.h"
#include "AssetManager.h"

Animation::Animation(const std::vector<std::string>& textureNames,
                     float frameDuration,
                     bool looping)
    : frameDuration(frameDuration),
      frameTimer(0.0f),
      currentFrame(0),
      finished(false),
      looping(looping)
{
    // Resolve each name into a texture pointer via AssetManager.
    frames.reserve(textureNames.size());
    for (const auto& name : textureNames)
    {
        frames.push_back(&AssetManager::get().getTexture(name));
    }
}

void Animation::update(float dt)
{
    // If we've finished (and we're not looping), do nothing.
    if (finished) return;
    if (frames.empty()) return;

    // Accumulate time and advance frames as needed.
    frameTimer += dt;
    while (frameTimer >= frameDuration)
    {
        frameTimer -= frameDuration;
        currentFrame++;

        if (currentFrame >= static_cast<int>(frames.size()))
        {
            if (looping)
            {
                currentFrame = 0;
            }
            else
            {
                // Hold on the last frame and flag finished.
                currentFrame = static_cast<int>(frames.size()) - 1;
                finished = true;
                break;
            }
        }
    }
}

sf::Texture& Animation::getCurrentTexture() const
{
    return *frames[currentFrame];
}

void Animation::reset()
{
    frameTimer = 0.0f;
    currentFrame = 0;
    finished = false;
}