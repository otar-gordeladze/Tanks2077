// EnemyTank.h - abstract base class for enemy tanks.

#pragma once

#include "Tank.h"

class EnemyTank : public Tank
{
protected:
    sf::Vector2f direction;
    float directionChangeInterval;
    float directionChangeTimer;
    int scoreValue;

    void pickRandomDirection();

public:
    // Constructor: position, hp, speed, AI change rate, score value.
    // No more sf::Color - the sprite is loaded by the subclass.
    EnemyTank(float x, float y, int hp, float speed,
              float changeInterval, int scoreValue);

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;

    int getScoreValue() const { return scoreValue; }
};









// EnemyTank.h - abstract base class for enemy tanks.
// Provides shared AI behavior; subclasses just configure stats and color.

// #pragma once

// #include "Tank.h"

// class EnemyTank : public Tank
// {
// protected:
//     // Current movement direction (dx, dy) - always normalized (length 1).
//     sf::Vector2f direction;

//     // How often (in seconds) the enemy spontaneously changes direction.
//     float directionChangeInterval;

//     // Time remaining until next spontaneous direction change.
//     float directionChangeTimer;

//     // Pick a random direction (one of 8 cardinal/diagonal directions).
//     void pickRandomDirection();

//     // Score awarded to the player when this enemy is destroyed.
//     int scoreValue;

// public:
//     // Constructor: position, color, hp, speed, and AI direction-change rate.
//     EnemyTank(float x, float y, sf::Color color, int hp, float speed,
//           float changeInterval, int scoreValue);
    
//     int getScoreValue() const { return scoreValue; }
    
//     // Concrete implementation of update - shared by all enemies.
//     // Moves the tank, handles wall collisions, occasionally changes direction.
//     void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;

    
//     // We DON'T need to override draw() or getBounds() — they're inherited from Tank.
// };