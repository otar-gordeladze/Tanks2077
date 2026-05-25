#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// main.cpp - Tanks 2077 - entry point
// Phase 0: minimal "hello SFML" - just opens a window and closes cleanly

int main()
{
    // Create a window 800x600 pixels with the title "Tanks 2077"
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Tanks 2077");

    // Main game loop - runs once per frame until window is closed
    while (window.isOpen())
    {
        // Process all events (mouse, keyboard, close button, etc.)
        while (auto event = window.pollEvent())
        {
            // If the user clicked the X button, close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Clear the screen to a dark gray color (R, G, B)
        window.clear(sf::Color(30, 30, 40));

        // (Drawing will happen here later - tanks, walls, bullets, etc.)

        // Show what we drew on screen (swap buffers)
        window.display();
    }

    return 0;
}