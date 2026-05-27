// Game.cpp - implementation of the Game class.
// This file contains the actual code; Game.h only declared what exists.
#include "PlayerTank.h"
#include "Game.h"
#include <algorithm>   // for std::remove_if
#include "Wall.h"
#include "Bullet.h"




// Constructor:
// Uses a member initializer list to construct the window directly.
// This is more efficient than assigning in the body because it constructs
// the window once with final values, instead of default-constructing then assigning.
Game::Game()
    : window(sf::VideoMode(sf::Vector2u(800, 600)), "Tanks 2077")
{
    
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

    // Step 3: show the new frame on screen (swap buffers).
    window.display();
}

void Game::handleCollisions()
{
    // For every bullet, check every wall.
    for (auto& obj1 : objects)
    {
        // Try to interpret obj1 as a Bullet
        Bullet* bullet = dynamic_cast<Bullet*>(obj1.get());
        if (bullet == nullptr || !bullet->isActive()) continue;

        for (auto& obj2 : objects)
        {
            // Try to interpret obj2 as a Wall
            Wall* wall = dynamic_cast<Wall*>(obj2.get());
            if (wall == nullptr || !wall->isActive()) continue;

            // SFML 3.x intersection check
            if (bullet->getBounds().findIntersection(wall->getBounds()).has_value())
            {
                // Bullet hits wall: damage the wall, kill the bullet
                wall->takeDamage(bullet->getDamage());
                bullet->setActive(false);
                break;   // this bullet is done; move to the next one
            }
        }
    }
}