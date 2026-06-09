// main.cpp - Tanks 2077 - entry point.
// Loads config + textures, then starts the Game.

#include "Game.h"
#include "Config.h"
#include "AssetManager.h"

int main()
{
    // 1. Load game settings.
    Config::get().loadFromFile("config/settings.txt");

    // 2. Pre-load all textures. Doing this once at startup means no disk I/O
    //    during gameplay -> smoother framerate.
    AssetManager::get().loadTexture("player_tank",   "assets/textures/player_tank.png");
    AssetManager::get().loadTexture("heavy_tank",    "assets/textures/heavy_tank.png");
    AssetManager::get().loadTexture("normal_tank",   "assets/textures/normal_tank.png");
    AssetManager::get().loadTexture("fast_tank",     "assets/textures/fast_tank.png");
    AssetManager::get().loadTexture("player_bullet", "assets/textures/player_bullet.png");
    AssetManager::get().loadTexture("heavy_bullet",  "assets/textures/heavy_bullet.png");
    AssetManager::get().loadTexture("normal_bullet", "assets/textures/normal_bullet.png");
    AssetManager::get().loadTexture("fast_bullet",   "assets/textures/fast_bullet.png");

    AssetManager::get().loadTexture("explosion_1", "assets/textures/explosion_1.png");
    AssetManager::get().loadTexture("explosion_2", "assets/textures/explosion_2.png");
    AssetManager::get().loadTexture("explosion_3", "assets/textures/explosion_3.png");
    AssetManager::get().loadTexture("explosion_4", "assets/textures/explosion_4.png");
    AssetManager::get().loadTexture("explosion_5", "assets/textures/explosion_5.png");
    AssetManager::get().loadTexture("explosion_6", "assets/textures/explosion_6.png");

    // 3. Run the game.
    Game game;
    game.run();
    return 0;
}