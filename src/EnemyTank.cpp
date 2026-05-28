// EnemyTank.cpp - implementation of shared enemy AI.

#include "EnemyTank.h"
#include "Wall.h"
#include <cmath>
#include <cstdlib>     // for std::rand
#include <ctime>       // for time-based RNG seeding

EnemyTank::EnemyTank(float x, float y, sf::Color color, int hp, float speed, float changeInterval)
    : Tank(x, y, color, hp, speed),
      direction(0.0f, 0.0f),
      directionChangeInterval(changeInterval),
      directionChangeTimer(changeInterval)
{
    pickRandomDirection();
}

void EnemyTank::pickRandomDirection()
{
    // Pick one of 8 directions: N, NE, E, SE, S, SW, W, NW.
    // rand() returns 0..RAND_MAX; % 8 gives 0..7.
    int dir = std::rand() % 8;

    // Each direction is 45 degrees apart.
    // The PI/4 is converted from degrees to radians.
    float angleRad = dir * (3.14159265f / 4.0f);
    direction.x = std::cos(angleRad);
    direction.y = std::sin(angleRad);

    // Round-off may give tiny non-zero values; normalize to be safe.
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f)
    {
        direction.x /= length;
        direction.y /= length;
    }
}

void EnemyTank::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    // --- 1. Tick the timer; change direction periodically ---
    directionChangeTimer -= dt;
    if (directionChangeTimer <= 0.0f)
    {
        pickRandomDirection();
        directionChangeTimer = directionChangeInterval;
    }

    // --- 2. Try to move; if blocked by wall, pick a new direction ---
    float newX = position.x + direction.x * speed * dt;
    float newY = position.y + direction.y * speed * dt;

    // Build a test rectangle at the new position.
    sf::FloatRect testBounds(sf::Vector2f(newX - 20.0f, newY - 20.0f),
                              sf::Vector2f(40.0f, 40.0f));

    bool blocked = false;
    for (const auto& obj : objects)
    {
        const Wall* wall = dynamic_cast<const Wall*>(obj.get());
        if (wall == nullptr || !wall->isActive()) continue;

        if (testBounds.findIntersection(wall->getBounds()).has_value())
        {
            blocked = true;
            break;
        }
    }

    // Also bounce off window edges (treat them as invisible walls)
    if (newX < 20.0f || newX > 780.0f || newY < 20.0f || newY > 580.0f)
        blocked = true;

    if (blocked)
    {
        // Pick a new direction and DON'T move this frame.
        pickRandomDirection();
    }
    else
    {
        position.x = newX;
        position.y = newY;
    }

    // --- 3. Update rotation to face the movement direction ---
    rotation = std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f;

    // --- 4. Sync the visual shape ---
    shape.setPosition(position);
    shape.setRotation(sf::degrees(rotation));
}