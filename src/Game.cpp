// Game.cpp - implementation of the Game class.
// This file contains the actual code; Game.h only declared what exists.
#include "PlayerTank.h"
#include "Game.h"
#include <algorithm>   // for std::remove_if
#include "Wall.h"
#include "Bullet.h"
#include "EnemyTypes.h"
#include <cstdlib>     // for srand
#include <ctime>       // for time
#include "EnemyTank.h"
#include "PlayerTank.h"
#include <string>     // for std::to_string
#include "BonusTypes.h"
#include "Bonus.h"
#include "Config.h"


// Constructor:
// Uses a member initializer list to construct the window directly.
// This is more efficient than assigning in the body because it constructs
// the window once with final values, instead of default-constructing then assigning.
Game::Game()
    : window(sf::VideoMode(sf::Vector2u(800, 600)), "Tanks 2077"),
    enemySpawnInterval(Config::get().getFloat("enemy_spawn_interval", 2.5f)),
    enemySpawnTimer(Config::get().getFloat("enemy_spawn_interval", 2.5f)),
    score(0),
    bonusSpawnInterval(Config::get().getFloat("bonus_spawn_interval", 8.0f)),
    bonusSpawnTimer(Config::get().getFloat("bonus_spawn_interval", 8.0f))


{
    // Seed random number generator using current time.
    // This is your "Introduction of randomness" rubric item.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Limit the framerate to 60 FPS - prevents the game from running too fast
    // on powerful machines and reduces unnecessary CPU usage.
    window.setFramerateLimit(60);
    // Create the player tank in the center of the window.
    // make_unique<PlayerTank>(400, 300) creates the object and returns a unique_ptr.
    // emplace_back moves it into the vector.
    // Even though the vector stores GameObject*, this works because PlayerTank IS-A GameObject.
    objects.emplace_back(std::make_unique<PlayerTank>(400.0f, 300.0f));
    // The objects vector starts empty. Subclasses (Tank, Wall...) will be
    // added in future phases via std::make_unique<...>().

    // Add a few walls around the map to test collision.
    objects.emplace_back(std::make_unique<Wall>(100.0f, 100.0f, 40.0f, 200.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(600.0f, 200.0f, 40.0f, 200.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(300.0f, 450.0f, 200.0f, 40.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(300.0f, 100.0f, 200.0f, 40.0f, 3));
    // Load the font for the HUD. Arial is always available on Windows.
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        // If font loading fails, we'll just have no text - not fatal.
        // (In a real product we'd log an error here.)
    }
    else
    {
        // Create the score and HP text objects now that the font is ready.
        // SFML 3.x: sf::Text requires a font reference in its constructor.
        scoreText.emplace(font, "Score: 0", 24);
        scoreText->setFillColor(sf::Color::White);
        scoreText->setPosition(sf::Vector2f(10.0f, 10.0f));

        hpText.emplace(font, "HP: 3", 24);
        hpText->setFillColor(sf::Color(80, 200, 80));
        hpText->setPosition(sf::Vector2f(10.0f, 40.0f));
    }
}

void Game::spawnRandomEnemy()
{
    // Pick a random spawn point along one of the four window edges.
    // Edge 0 = top, 1 = right, 2 = bottom, 3 = left.
    int edge = std::rand() % 4;
    float x = 0.0f, y = 0.0f;
    switch (edge)
    {
        case 0: x = static_cast<float>(std::rand() % 760 + 20); y = 30.0f;  break;
        case 1: x = 770.0f; y = static_cast<float>(std::rand() % 560 + 20); break;
        case 2: x = static_cast<float>(std::rand() % 760 + 20); y = 570.0f; break;
        case 3: x = 30.0f;  y = static_cast<float>(std::rand() % 560 + 20); break;
    }

    // Pick a random enemy type. We use weighted probabilities:
    //   50% Normal, 30% Fast, 20% Heavy.
    int roll = std::rand() % 10;
    if (roll < 5)
        objects.emplace_back(std::make_unique<NormalEnemy>(x, y));
    else if (roll < 8)
        objects.emplace_back(std::make_unique<FastEnemy>(x, y));
    else
        objects.emplace_back(std::make_unique<HeavyEnemy>(x, y));
}

// The main game loop - called once from main() and runs until window closes.
void Game::run()
{
    clock.restart();
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

// Handle window events (close button, keyboard, mouse, etc.)
void Game::processEvents()
{
    // pollEvent returns std::optional<sf::Event> - it's "empty" when no events left.
    // The "auto" lets the compiler figure out the exact type.
    while (auto event = window.pollEvent())
    {
        // event->is<sf::Event::Closed>() returns true if it was the X button.
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

// Update every object in the game by dt seconds.
void Game::update(float dt)
{
    // 1. Update all objects
    size_t initialSize = objects.size();
    for (size_t i = 0; i < initialSize; ++i)
    {
        objects[i]->update(dt, objects);
    }

    // --- Spawn enemies over time ---
    enemySpawnTimer -= dt;
    if (enemySpawnTimer <= 0.0f)
    {
        spawnRandomEnemy();
        enemySpawnTimer = enemySpawnInterval;
    }

    // --- Spawn bonuses over time ---
    bonusSpawnTimer -= dt;
    if (bonusSpawnTimer <= 0.0f)
    {
        spawnRandomBonus();
        bonusSpawnTimer = bonusSpawnInterval;
    }
    // 2. Detect & resolve collisions (bullets vs walls)
    handleCollisions();

    // 3. Remove inactive objects
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return !obj->isActive();
            }),
        objects.end()
    );
}


// Draw every object to the window.
void Game::render()
{
    // Step 1: clear the previous frame to dark gray.
    window.clear(sf::Color(30, 30, 40));

    // Step 2: draw all game objects.
    for (auto& obj : objects)
    {
        obj->draw(window);
    }
    drawHUD();
    // Step 3: show the new frame on screen (swap buffers).
    window.display();
}

void Game::handleCollisions()
{
    // --- 1. Find the player (if still alive) ---
    // We only have one player, so a single pass is fine.
    PlayerTank* player = nullptr;
    for (auto& obj : objects)
    {
        player = dynamic_cast<PlayerTank*>(obj.get());
        if (player != nullptr) break;
    }

    // --- 2. For each bullet, check what it hits ---
    for (auto& obj1 : objects)
    {
        Bullet* bullet = dynamic_cast<Bullet*>(obj1.get());
        if (bullet == nullptr || !bullet->isActive()) continue;

        for (auto& obj2 : objects)
        {
            if (!obj2->isActive()) continue;
            if (obj2.get() == bullet) continue;     // a bullet doesn't hit itself

            // Bullet vs Wall
            if (Wall* wall = dynamic_cast<Wall*>(obj2.get()))
            {
                if (bullet->getBounds().findIntersection(wall->getBounds()).has_value())
                {
                    wall->takeDamage(bullet->getDamage());
                    bullet->setActive(false);
                    break;
                }
            }
            // Bullet vs Enemy
            // Bullet vs Enemy
            else if (EnemyTank* enemy = dynamic_cast<EnemyTank*>(obj2.get()))
            {
                if (bullet->getBounds().findIntersection(enemy->getBounds()).has_value())
                {
                    enemy->takeDamage(bullet->getDamage());
                    bullet->setActive(false);

                    // If the hit killed the enemy, award score.
                    if (!enemy->isActive())
                        score += enemy->getScoreValue();

                    break;
                }
            }
        }
    }

    // --- 3. Enemy touches Player ---
    if (player != nullptr && player->isActive())
    {
        for (auto& obj : objects)
        {
            EnemyTank* enemy = dynamic_cast<EnemyTank*>(obj.get());
            if (enemy == nullptr || !enemy->isActive()) continue;

            if (player->getBounds().findIntersection(enemy->getBounds()).has_value())
            {
                player->tryDamage(1);   // protected by the invulnerability timer
                // We DON'T break - other enemies could also be touching, but
                // the invulnerability means only one will register damage.
            }
        }
        // --- 4. Player picks up Bonus ---
    if (player != nullptr && player->isActive())
    {
        for (auto& obj : objects)
        {
            Bonus* bonus = dynamic_cast<Bonus*>(obj.get());
            if (bonus == nullptr || !bonus->isActive()) continue;

            if (player->getBounds().findIntersection(bonus->getBounds()).has_value())
            {
                bonus->apply(*player);     // POLYMORPHIC CALL !!!
                bonus->setActive(false);    // bonus disappears after pickup
            }
        }
    }
    }

    // --- 4. Close the window if player is dead ---
    // (Temporary - we'll add a proper game-over screen in Phase 7.)
    if (player == nullptr || !player->isActive())
    {
        window.close();
    }
}

void Game::drawHUD()
{
    // Find the player (so we can read their HP).
    int playerHP = 0;
    for (auto& obj : objects)
    {
        if (PlayerTank* p = dynamic_cast<PlayerTank*>(obj.get()))
        {
            playerHP = p->getHP();      // inherited from Tank
            break;
        }
    }

    // Update text contents and draw, if font loaded successfully.
    if (scoreText.has_value())
    {
        scoreText->setString("Score: " + std::to_string(score));
        window.draw(*scoreText);
    }

    if (hpText.has_value())
    {
        hpText->setString("HP: " + std::to_string(playerHP));
        window.draw(*hpText);
    }
}


void Game::spawnRandomBonus()
{
    // Random position in the playable area (avoiding edges).
    float x = static_cast<float>(std::rand() % 700 + 50);
    float y = static_cast<float>(std::rand() % 500 + 50);

    // Pick one of three bonus types with equal probability.
    int type = std::rand() % 3;
    switch (type)
    {
        case 0: objects.emplace_back(std::make_unique<ShieldBonus>(x, y));        break;
        case 1: objects.emplace_back(std::make_unique<FastShootBonus>(x, y));     break;
        case 2: objects.emplace_back(std::make_unique<FastMovementBonus>(x, y));  break;
    }
}