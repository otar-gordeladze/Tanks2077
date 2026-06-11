// AssetManager.h - singleton that loads and stores textures (and could store
// fonts, sounds, etc.) for the lifetime of the game.
// Sprites only POINT to textures - the textures must outlive them, so storing
// them centrally here is the cleanest approach.

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

class AssetManager
{
private:
    // Map of texture-name -> loaded texture.
    // We use unique_ptr because sf::Texture is not copyable/movable in some SFML versions,
    // and storing them by pointer keeps the addresses stable when the map grows.
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;

    // Private constructor - this is a singleton.
    AssetManager() = default;

    // Map of sound-name -> loaded sound buffer.
std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers;

public:
    // Singletons cannot be copied or assigned.
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    // Access the single instance.
    static AssetManager& get();

    // Load a texture from disk and store it under the given name.
    // Returns true on success. Safe to call multiple times - subsequent calls
    // with the same name are no-ops.
    bool loadTexture(const std::string& name, const std::string& path);

    // Get a reference to a previously-loaded texture. Throws if not found.
    // (Throwing is intentional here: a missing texture is a bug we want to know about.)
    sf::Texture& getTexture(const std::string& name);

    // Check if a texture is loaded (useful for fallback logic).
    bool hasTexture(const std::string& name) const;

    // Load a sound buffer from disk and store it under the given name.
bool loadSound(const std::string& name, const std::string& path);

// Get a reference to a previously-loaded sound buffer.
sf::SoundBuffer& getSound(const std::string& name);

// Check if a sound is loaded.
bool hasSound(const std::string& name) const;
};