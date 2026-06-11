// EnemyTank.cpp - shared enemy AI: random wandering + occasional firing.

#include "EnemyTank.h"
#include "Wall.h"
#include <cmath>
#include <cstdlib>
#include "Game.h"

EnemyTank::EnemyTank(float x, float y, int hp, float speed,
                     float changeInterval, int scoreValue,
                     const std::string& bulletTextureName,
                     float aiShootInterval)
    : Tank(x, y, hp, speed, bulletTextureName, aiShootInterval, true, "shoot"),  // is enemy
      direction(0.0f, 0.0f),
      directionChangeInterval(changeInterval),
      directionChangeTimer(changeInterval),
      scoreValue(scoreValue),
      shootTimer(aiShootInterval),
      aiShootInterval(aiShootInterval)
{
    pickRandomDirection();
}

void EnemyTank::pickRandomDirection()
{
    int dir = std::rand() % 8;
    float angleRad = dir * (3.14159265f / 4.0f);
    direction.x = std::cos(angleRad);
    direction.y = std::sin(angleRad);

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f)
    {
        direction.x /= length;
        direction.y /= length;
    }
}

void EnemyTank::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    // Tick down the shared shootCooldown (inherited from Tank).
    if (shootCooldown > 0.0f) shootCooldown -= dt;

    // --- Direction-change timer ---
    directionChangeTimer -= dt;
    if (directionChangeTimer <= 0.0f)
    {
        pickRandomDirection();
        directionChangeTimer = directionChangeInterval;
    }

    // --- Try to move; if blocked by a wall, pick a new direction ---
    float newX = position.x + direction.x * speed * dt;
    float newY = position.y + direction.y * speed * dt;

    const float HALF = 70.0f * 0.7f / 2.0f;
    const float SIZE = 70.0f * 0.7f;
    sf::FloatRect testBounds(sf::Vector2f(newX - HALF, newY - HALF),
                          sf::Vector2f(SIZE, SIZE));

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
    if (newX < 20.0f || newX > (WINDOW_WIDTH - 20.0f) ||
        newY < 20.0f || newY > (WINDOW_HEIGHT - 20.0f))
        blocked = true;

    if (blocked)
    {
        pickRandomDirection();
    }
    else
    {
        position.x = newX;
        position.y = newY;
    }

    rotation = std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f;

    if (sprite.has_value())
    {
        sprite->setPosition(position);
        sprite->setRotation(sf::degrees(rotation));
    }

    // --- AI shooting ---
    // Counts down toward 0; when it hits 0, fire and reset (with a small random
    // variation so all enemies don't shoot in sync).
    shootTimer -= dt;
    if (shootTimer <= 0.0f)
    {
        shoot(objects);   // inherited from Tank
        // Re-arm with a randomized interval ±30%.
        float jitter = ((std::rand() % 60) - 30) / 100.0f;     // -0.30 ... +0.29
        shootTimer = aiShootInterval * (1.0f + jitter);
    }
}