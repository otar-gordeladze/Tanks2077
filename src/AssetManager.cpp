// AssetManager.cpp - implementation of the texture loader/cache.

#include "AssetManager.h"
#include <iostream>     // for error logging
#include <stdexcept>    // for std::runtime_error

AssetManager& AssetManager::get()
{
    static AssetManager instance;
    return instance;
}

bool AssetManager::loadTexture(const std::string& name, const std::string& path)
{
    // If we've already loaded this name, don't reload.
    if (textures.find(name) != textures.end())
        return true;

    // Allocate a new texture on the heap and load from file.
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(path))
    {
        std::cerr << "AssetManager: failed to load texture: " << path << std::endl;
        return false;
    }

    // SFML 3.x: enable smooth scaling. Otherwise sprite scaling looks pixelated.
    texture->setSmooth(true);

    // Move the unique_ptr into the map (transferring ownership).
    textures[name] = std::move(texture);
    return true;
}

sf::Texture& AssetManager::getTexture(const std::string& name)
{
    auto it = textures.find(name);
    if (it == textures.end())
    {
        // Loud error - this means a typo or missing loadTexture call.
        throw std::runtime_error("AssetManager: texture not loaded: " + name);
    }
    return *(it->second);
}

bool AssetManager::hasTexture(const std::string& name) const
{
    return textures.find(name) != textures.end();
}


bool AssetManager::loadSound(const std::string& name, const std::string& path)
{
    if (soundBuffers.find(name) != soundBuffers.end())
        return true;

    auto buffer = std::make_unique<sf::SoundBuffer>();
    if (!buffer->loadFromFile(path))
    {
        std::cerr << "AssetManager: failed to load sound: " << path << std::endl;
        return false;
    }

    soundBuffers[name] = std::move(buffer);
    return true;
}

sf::SoundBuffer& AssetManager::getSound(const std::string& name)
{
    auto it = soundBuffers.find(name);
    if (it == soundBuffers.end())
        throw std::runtime_error("AssetManager: sound not loaded: " + name);
    return *(it->second);
}

bool AssetManager::hasSound(const std::string& name) const
{
    return soundBuffers.find(name) != soundBuffers.end();
}