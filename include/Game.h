// Game.h - the central class that supervises the entire game.
// Owns the window, the clock, and the list of all game objects.
// Required by the assignment: "Creating base class Game that supervises the course of the game"
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>     // for std::unique_ptr
#include <vector>     // for std::vector
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include "Scoreboard.h"

// All the states the Game can be in. Only one is active at a time.
enum class GameState
{
    Menu,
    Playing,
    GameOver
};

// Window dimensions - referenced throughout the codebase.
constexpr int WINDOW_WIDTH  = 1024;
constexpr int WINDOW_HEIGHT = 768;

class Game
{
private:
    // The SFML window the game is drawn into.
    sf::RenderWindow window;

    // SFML clock - measures time between frames for frame-independent movement.
    sf::Clock clock;
    
    

    // Font used by all text in the HUD.
    sf::Font font;

    // HUD text objects (created once, updated each frame).
    // We use std::optional because sf::Text requires a font in its constructor,
    // and we can't construct it in the initializer list cleanly.
    std::optional<sf::Text> scoreText;
    std::optional<sf::Text> hpText;

    
    // Player's current score.
    // Text used for the menu and game-over screens.
    std::optional<sf::Text> titleText;
    std::optional<sf::Text> menuPromptText;
    std::optional<sf::Text> gameOverText;
    std::optional<sf::Text> finalScoreText;
    std::optional<sf::Text> restartPromptText;
    std::optional<sf::Text> scoreboardText;
    
    // Time between random enemy spawns (in seconds).
    float enemySpawnInterval;

    // Time remaining until next enemy spawn.
    float enemySpawnTimer;

    int score;

    // Bonus spawning timers.
    float bonusSpawnInterval;
    float bonusSpawnTimer;

    // Persistent high-scores list (loaded at startup, saved on game-over).
    Scoreboard scoreboard;

    // Current state of the game (controls what's drawn and what events do).
    GameState state;

    // Current level number. Starts at 1; advances at score thresholds.
    int currentLevel;

    // Banner shown briefly when the level changes ("LEVEL 2!").
    std::optional<sf::Text> levelBannerText;
    std::optional<sf::Text> levelHudText;
    // Background image drawn first each frame (under everything).
    std::optional<sf::Sprite> backgroundSprite;


    // Seconds remaining for the banner to be visible. <= 0 means hidden.
    float levelBannerTimer;
    
    // THE polymorphic container required by the assignment.
    // It holds unique_ptr to GameObject - the BASE class - so it can store
    // ANY subclass (Tank, Wall, Bullet, Bonus...) all together.
    // unique_ptr = automatic memory management: when the vector is destroyed,
    // every contained object is destroyed too. No "delete" calls needed.
    std::vector<std::unique_ptr<GameObject>> objects;

    // --- Private helper methods (the steps of one frame) ---

    // Per-state event handlers

    // Read keyboard, mouse, close button.
    void processEvents();
    void processMenuEvents();
    void processPlayingEvents();
    void processGameOverEvents();

    // Per-state update / render

    // Update every object's logic.
    void update(float dt);

    // Draw every object to the window.
    void render();
    void renderMenu();
    void renderGameOver();

    // Helpers (only used during Playing)

    // Spawn a random enemy type at a random edge of the map.
    void spawnRandomEnemy();
    // Spawn a random bonus type at a random position on the map.
    void spawnRandomBonus();
    // Detect collisions between bullets and walls; mark them inactive accordingly.
    void handleCollisions();
    void drawHUD();

    // Reset the world to a fresh playing state.
    void startNewGame();

    // Replace walls with the layout for the given level.
    void loadLevel(int level);

    // Spawn the wall layout for Level 1.
    void spawnWallsLevel1();

    // Spawn the wall layout for Level 2.
    void spawnWallsLevel2();

    // Check if we should advance to a higher level based on score.
    void checkLevelProgression();

    

public:
    // Constructor - creates the window and sets up initial state.
    Game();

    int getLevel() const { return currentLevel; }

    // The main game loop - call this from main() to start the game.
    // Runs until the user closes the window.
    void run();
};
