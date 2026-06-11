// SoundPlayer.cpp - implementation of the sound voice pool.

#include "SoundPlayer.h"
#include "AssetManager.h"
#include <iostream>     // (already included probably, but in case not)


SoundPlayer::SoundPlayer()
    : pool(POOL_SIZE),
      nextVoice(0),
      volume(70.0f),
      musicVolume(40.0f)    // background music quieter than SFX
{
}

SoundPlayer& SoundPlayer::get()
{
    static SoundPlayer instance;
    return instance;
}

void SoundPlayer::play(const std::string& soundName)
{
    // Safety: skip silently if the buffer isn't loaded.
    if (!AssetManager::get().hasSound(soundName))
        return;

    sf::SoundBuffer& buffer = AssetManager::get().getSound(soundName);

    // Grab the next voice in the pool.
    auto& voiceSlot = pool[nextVoice];

    // Construct (or reconstruct) the Sound on this buffer.
    voiceSlot.emplace(buffer);
    voiceSlot->setVolume(volume);
    voiceSlot->play();

    // Advance round-robin index.
    nextVoice = (nextVoice + 1) % POOL_SIZE;
}

void SoundPlayer::setVolume(float v)
{
    volume = v;
}


void SoundPlayer::playMusic(const std::string& path)
{
    // Construct (or reconstruct) the music object on the file.
    // SFML 3.x: sf::Music constructor takes a path and throws on failure,
    // so we use openFromFile via a temporary that we move into the optional.
    music.emplace();
    if (!music->openFromFile(path))
    {
        // Failed to open - clear the optional and bail.
        music.reset();
        return;
    }

    music->setLooping(true);     // loop forever
    music->setVolume(musicVolume);
    music->play();
}

void SoundPlayer::stopMusic()
{
    if (music.has_value())
        music->stop();
}

void SoundPlayer::setMusicVolume(float v)
{
    musicVolume = v;
    if (music.has_value())
        music->setVolume(musicVolume);
}