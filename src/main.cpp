// main.cpp - Tanks 2077 - entry point
// All the real logic lives in the Game class. This file just kicks it off.

#include "Game.h"

int main()
{
    Game game;     // Create the Game (constructor opens the window).
    game.run();    // Run the main loop until window closes.
    return 0;      // Return 0 = "program ended successfully"
}