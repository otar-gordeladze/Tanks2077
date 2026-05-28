// Game.h - the central class that supervises the entire game.
// Owns the window, the clock, and the list of all game objects.
// Required by the assignment: "Creating base class Game that supervises the course of the game"
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>     // for std::unique_ptr
#include <vector>     // for std::vector
#include "GameObject.h"

class Game
{
private:
    // Time between random enemy spawns (in seconds).
    float enemySpawnInterval;

    // Time remaining until next enemy spawn.
    float enemySpawnTimer;


    // The SFML window the game is drawn into.
    sf::RenderWindow window;

    // SFML clock - measures time between frames for frame-independent movement.
    sf::Clock clock;
    
    
    // Detect collisions between bullets and walls; mark them inactive accordingly.
    void handleCollisions();
    
    
    // THE polymorphic container required by the assignment.
    // It holds unique_ptr to GameObject - the BASE class - so it can store
    // ANY subclass (Tank, Wall, Bullet, Bonus...) all together.
    // unique_ptr = automatic memory management: when the vector is destroyed,
    // every contained object is destroyed too. No "delete" calls needed.
    std::vector<std::unique_ptr<GameObject>> objects;

    // --- Private helper methods (the steps of one frame) ---

    // Read keyboard, mouse, close button.
    void processEvents();

    // Update every object's logic.
    void update(float dt);

    // Draw every object to the window.
    void render();

    // Spawn a random enemy type at a random edge of the map.
    void spawnRandomEnemy();

public:
    // Constructor - creates the window and sets up initial state.
    Game();

    // The main game loop - call this from main() to start the game.
    // Runs until the user closes the window.
    void run();
};
