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
#include "Explosion.h"
#include "SoundPlayer.h"



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
    bonusSpawnTimer(Config::get().getFloat("bonus_spawn_interval", 8.0f)),
    scoreboard(),
    state(GameState::Menu),
    currentLevel(1),
    levelBannerTimer(0.0f)

{
    // Seed random number generator using current time.
    // This is your "Introduction of randomness" rubric item.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Load existing high scores (if any).
    scoreboard.loadFromFile();

    // Limit the framerate to 60 FPS - prevents the game from running too fast
    // on powerful machines and reduces unnecessary CPU usage.
    window.setFramerateLimit(60);
    // The objects vector starts empty. Subclasses (Tank, Wall...) will be
    // added in future phases via std::make_unique<...>().

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

        levelHudText.emplace(font, "Level: 1", 24);
        levelHudText->setFillColor(sf::Color(80, 220, 255));
        levelHudText->setPosition(sf::Vector2f(10.0f, 70.0f));

        // --- Menu screen ---
        titleText.emplace(font, "TANKS 2077", 72);
        titleText->setFillColor(sf::Color(80, 220, 255));    // cyan
        // Center the text by accounting for its width.
        sf::FloatRect tb = titleText->getLocalBounds();
        titleText->setOrigin(sf::Vector2f(tb.size.x / 2.0f, tb.size.y / 2.0f));
        titleText->setPosition(sf::Vector2f(400.0f, 220.0f));

        menuPromptText.emplace(font, "Press ENTER to start    -    ESC to quit", 22);
        menuPromptText->setFillColor(sf::Color(200, 200, 200));
        sf::FloatRect mb = menuPromptText->getLocalBounds();
        menuPromptText->setOrigin(sf::Vector2f(mb.size.x / 2.0f, mb.size.y / 2.0f));
        menuPromptText->setPosition(sf::Vector2f(400.0f, 360.0f));

        // --- Game over screen ---
        gameOverText.emplace(font, "GAME OVER", 64);
        gameOverText->setFillColor(sf::Color(255, 80, 100));
        sf::FloatRect gb = gameOverText->getLocalBounds();
        gameOverText->setOrigin(sf::Vector2f(gb.size.x / 2.0f, gb.size.y / 2.0f));
        gameOverText->setPosition(sf::Vector2f(400.0f, 200.0f));

        finalScoreText.emplace(font, "Final Score: 0", 32);
        finalScoreText->setFillColor(sf::Color::White);
        finalScoreText->setPosition(sf::Vector2f(400.0f, 290.0f));    // origin set when shown

        restartPromptText.emplace(font, "ENTER to restart    -    ESC to quit", 22);
        restartPromptText->setFillColor(sf::Color(200, 200, 200));
        sf::FloatRect rb = restartPromptText->getLocalBounds();
        restartPromptText->setOrigin(sf::Vector2f(rb.size.x / 2.0f, rb.size.y / 2.0f));
        restartPromptText->setPosition(sf::Vector2f(400.0f, 380.0f));
        // --- Scoreboard text (shown on Menu) ---
        scoreboardText.emplace(font, "HIGH SCORES", 22);
        scoreboardText->setFillColor(sf::Color(180, 180, 220));
        scoreboardText->setPosition(sf::Vector2f(400.0f, 420.0f));    // origin set in renderMenu
        // --- Level banner ---
        levelBannerText.emplace(font, "LEVEL 2", 64);
        levelBannerText->setFillColor(sf::Color(255, 220, 80));
        sf::FloatRect lb = levelBannerText->getLocalBounds();
        levelBannerText->setOrigin(sf::Vector2f(lb.size.x / 2.0f, lb.size.y / 2.0f));
        levelBannerText->setPosition(sf::Vector2f(400.0f, 300.0f));
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
    // Dispatch to the correct per-state handler.
    switch (state)
    {
        case GameState::Menu:     processMenuEvents();     break;
        case GameState::Playing:  processPlayingEvents();  break;
        case GameState::GameOver: processGameOverEvents(); break;
    }
}

void Game::processMenuEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (auto* k = event->getIf<sf::Event::KeyPressed>())
        {
            if (k->code == sf::Keyboard::Key::Enter)
                startNewGame();
            else if (k->code == sf::Keyboard::Key::Escape)
                window.close();
        }
    }
}

void Game::processPlayingEvents()
{
    // Same as the old processEvents.
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Game::processGameOverEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (auto* k = event->getIf<sf::Event::KeyPressed>())
        {
            if (k->code == sf::Keyboard::Key::Enter)
                startNewGame();
            else if (k->code == sf::Keyboard::Key::Escape)
                window.close();
        }
    }
}

// Update every object in the game by dt seconds.
void Game::update(float dt)
{
    // Only run game logic during Playing state.
    if (state != GameState::Playing) return;

    // --- Update all objects ---
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

    // --- Detect & resolve collisions ---
    handleCollisions();

    // Check if we should advance to a higher level.
    checkLevelProgression();

    // Tick down the level banner timer.
    if (levelBannerTimer > 0.0f)
        levelBannerTimer -= dt;

    // --- Remove inactive objects ---
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
    // Same dark background for all states.
    window.clear(sf::Color(30, 30, 40));

    switch (state)
    {
        case GameState::Menu:
            renderMenu();
            break;

        case GameState::Playing:
            for (auto& obj : objects) obj->draw(window);
            drawHUD();
            break;

        case GameState::GameOver:
            // Show the (frozen) game world behind, then the overlay.
            for (auto& obj : objects) obj->draw(window);
            drawHUD();
            // Show the level banner overlay if active.
            if (levelBannerTimer > 0.0f && levelBannerText.has_value())
            {
                // Fade out: alpha decreases as timer approaches 0.
                float alpha = std::min(1.0f, levelBannerTimer / 2.0f) * 255.0f;
                levelBannerText->setString("LEVEL " + std::to_string(currentLevel));

                sf::FloatRect lb = levelBannerText->getLocalBounds();
                levelBannerText->setOrigin(sf::Vector2f(lb.size.x / 2.0f, lb.size.y / 2.0f));
                levelBannerText->setPosition(sf::Vector2f(400.0f, 300.0f));

                sf::Color c = sf::Color(255, 220, 80, static_cast<std::uint8_t>(alpha));
                levelBannerText->setFillColor(c);
                window.draw(*levelBannerText);
            }
            renderGameOver();
            break;
    }

    window.display();
}

void Game::renderMenu()
{
    if (titleText.has_value())      window.draw(*titleText);
    if (menuPromptText.has_value()) window.draw(*menuPromptText);

    // --- Scoreboard display ---
    if (scoreboardText.has_value())
    {
        // Build a multi-line string: "HIGH SCORES\n1. 1200\n2. 800\n..."
        std::string text = "HIGH SCORES\n";
        const auto& scores = scoreboard.getScores();
        if (scores.empty())
        {
            text += "(none yet)";
        }
        else
        {
            for (size_t i = 0; i < scores.size(); ++i)
            {
                text += std::to_string(i + 1) + ". " + std::to_string(scores[i]) + "\n";
            }
        }
        scoreboardText->setString(text);

        // Center horizontally each time (text may grow with new entries).
        sf::FloatRect b = scoreboardText->getLocalBounds();
        scoreboardText->setOrigin(sf::Vector2f(b.size.x / 2.0f, 0.0f));
        scoreboardText->setPosition(sf::Vector2f(400.0f, 420.0f));

        window.draw(*scoreboardText);
    }
}

void Game::renderGameOver()
{
    // Dim background.
    sf::RectangleShape overlay(sf::Vector2f(800.0f, 600.0f));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    if (gameOverText.has_value()) window.draw(*gameOverText);

    if (finalScoreText.has_value())
    {
        // Indicate new high score if applicable.
        // (We call isHighScore on the score BEFORE it was added - but we already
        // added it in handleCollisions. So instead check: is our score in the list?)
        bool newHigh = false;
        const auto& list = scoreboard.getScores();
        if (!list.empty() && list.front() == score)
            newHigh = true;

        std::string msg = "Final Score: " + std::to_string(score);
        if (newHigh) msg += "   NEW HIGH SCORE!";

        finalScoreText->setString(msg);
        sf::FloatRect b = finalScoreText->getLocalBounds();
        finalScoreText->setOrigin(sf::Vector2f(b.size.x / 2.0f, b.size.y / 2.0f));
        finalScoreText->setPosition(sf::Vector2f(400.0f, 290.0f));
        finalScoreText->setFillColor(newHigh ? sf::Color(255, 220, 80) : sf::Color::White);
        window.draw(*finalScoreText);
    }

    if (restartPromptText.has_value()) window.draw(*restartPromptText);
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
            // Bullet vs Enemy (player bullets only - no friendly fire)
            else if (EnemyTank* enemy = dynamic_cast<EnemyTank*>(obj2.get()))
            {
                if (bullet->isFromEnemy()) continue;    // skip enemy bullets vs enemies
                if (bullet->getBounds().findIntersection(enemy->getBounds()).has_value())
                {
                    enemy->takeDamage(bullet->getDamage());
                    bullet->setActive(false);

                    // If the hit killed the enemy, award score AND spawn an explosion
                    // at the enemy's position. Note: we use the enemy's position,
                    // not the bullet's, so the explosion is centered on what died.
                    if (!enemy->isActive())
                    {
                        score += enemy->getScoreValue();
                        sf::Vector2f enemyPos = enemy->getPosition();
                        objects.emplace_back(
                            std::make_unique<Explosion>(enemyPos.x, enemyPos.y));
                        SoundPlayer::get().play("explosion");
                    }

                    break;
                }
            }
        }
    }

    // --- 2.5. Enemy bullets hit Player ---
    if (player != nullptr && player->isActive())
    {
        for (auto& obj : objects)
        {
            Bullet* bullet = dynamic_cast<Bullet*>(obj.get());
            if (bullet == nullptr || !bullet->isActive()) continue;
            if (!bullet->isFromEnemy()) continue;     // only enemy bullets damage player

            if (player->getBounds().findIntersection(bullet->getBounds()).has_value())
            {
                player->tryDamage(bullet->getDamage());
                bullet->setActive(false);
                SoundPlayer::get().play("pickup");
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
    // --- 4. If player is dead, transition to GameOver state ---
    if (player == nullptr || !player->isActive())
    {
        // Record this score (saves to file automatically).
        scoreboard.addScore(score);
        state = GameState::GameOver;
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
    // Show level in HUD.
    if (scoreText.has_value())
    {
        // Reuse scoreText style for level (we don't need a separate text object - small touch)
        std::string levelMsg = "Level: " + std::to_string(currentLevel);
        if (levelHudText.has_value())
        {
            levelHudText->setString("Level: " + std::to_string(currentLevel));
            window.draw(*levelHudText);
        }
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

void Game::startNewGame()
{
    objects.clear();
    score = 0;
    currentLevel = 1;
    levelBannerTimer = 0.0f;
    enemySpawnTimer = enemySpawnInterval;
    bonusSpawnTimer = bonusSpawnInterval;

    objects.emplace_back(std::make_unique<PlayerTank>(400.0f, 300.0f));
    loadLevel(1);

    state = GameState::Playing;
    clock.restart();
}


void Game::loadLevel(int level)
{
    // Remove existing walls (other objects survive: player, enemies, bullets, bonuses).
    // We use erase-remove with a lambda just like the cleanup of inactive objects.
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return dynamic_cast<Wall*>(obj.get()) != nullptr;
            }),
        objects.end()
    );

    // Spawn the new layout.
    switch (level)
    {
        case 1: spawnWallsLevel1(); break;
        case 2: spawnWallsLevel2(); break;
        default: spawnWallsLevel1(); break;     // safety fallback
    }

    currentLevel = level;
}

void Game::spawnWallsLevel1()
{
    // Original 4-wall layout: open arena.
    objects.emplace_back(std::make_unique<Wall>(100.0f, 100.0f, 40.0f, 200.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(600.0f, 200.0f, 40.0f, 200.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(300.0f, 450.0f, 200.0f, 40.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(300.0f, 100.0f, 200.0f, 40.0f, 3));
}

void Game::spawnWallsLevel2()
{
    // Tighter layout: more walls, corridors, harder to navigate.
    // Four corner pillars
    objects.emplace_back(std::make_unique<Wall>(80.0f,  80.0f,  40.0f, 100.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(80.0f,  420.0f, 40.0f, 100.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(680.0f, 80.0f,  40.0f, 100.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(680.0f, 420.0f, 40.0f, 100.0f, 3));

    // Two horizontal walls forming corridors
    objects.emplace_back(std::make_unique<Wall>(200.0f, 200.0f, 160.0f, 40.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(440.0f, 360.0f, 160.0f, 40.0f, 3));

    // Two vertical center pillars
    objects.emplace_back(std::make_unique<Wall>(380.0f, 100.0f, 40.0f, 120.0f, 3));
    objects.emplace_back(std::make_unique<Wall>(380.0f, 380.0f, 40.0f, 120.0f, 3));
}

void Game::checkLevelProgression()
{
    // Level 1 -> Level 2 at score 777.
    if (currentLevel == 1 && score >= 80)
    {
        loadLevel(2);
        // Show banner for 2 seconds.
        levelBannerTimer = 2.0f;
    }
    // Future: add more thresholds here (Level 3, 4...)
}