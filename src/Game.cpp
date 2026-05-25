// Game.cpp - implementation of the Game class.
// This file contains the actual code; Game.h only declared what exists.

#include "Game.h"

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

    // The objects vector starts empty. Subclasses (Tank, Wall...) will be
    // added in future phases via std::make_unique<...>().
}

// The main game loop - called once from main() and runs until window closes.
void Game::run()
{
    // Restart the clock so the first frame's dt isn't huge.
    clock.restart();

    while (window.isOpen())
    {
        // --- 1. Measure time elapsed since last frame ---
        // clock.restart() returns elapsed time AND resets the clock to 0.
        // asSeconds() gives a float like 0.0166 for ~60fps.
        float dt = clock.restart().asSeconds();

        // --- 2. The three steps of every frame ---
        processEvents();   // Read input
        update(dt);        // Update game logic
        render();          // Draw everything
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
    // Range-based for loop over the polymorphic vector.
    // "auto&" means "let the compiler deduce the type, give me a reference"
    // (so we don't COPY the unique_ptr, which would be illegal anyway).
    for (auto& obj : objects)
    {
        // obj is unique_ptr<GameObject>; obj->update(dt) calls the right
        // version automatically thanks to the "virtual" keyword.
        // This is the POLYMORPHISM in action!
        obj->update(dt);
    }

    // (Later: remove inactive objects here)
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