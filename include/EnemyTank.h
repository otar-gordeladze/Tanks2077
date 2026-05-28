// EnemyTank.h - abstract base class for enemy tanks.
// Provides shared AI behavior; subclasses just configure stats and color.

#pragma once

#include "Tank.h"

class EnemyTank : public Tank
{
protected:
    // Current movement direction (dx, dy) - always normalized (length 1).
    sf::Vector2f direction;

    // How often (in seconds) the enemy spontaneously changes direction.
    float directionChangeInterval;

    // Time remaining until next spontaneous direction change.
    float directionChangeTimer;

    // Pick a random direction (one of 8 cardinal/diagonal directions).
    void pickRandomDirection();

public:
    // Constructor: position, color, hp, speed, and AI direction-change rate.
    EnemyTank(float x, float y, sf::Color color, int hp, float speed, float changeInterval);

    // Concrete implementation of update - shared by all enemies.
    // Moves the tank, handles wall collisions, occasionally changes direction.
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;

    // We DON'T need to override draw() or getBounds() — they're inherited from Tank.
};