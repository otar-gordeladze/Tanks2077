// PlayerTank.cpp - implementation of the player-controlled tank.

#include "PlayerTank.h"
#include "Bullet.h"
#include <cmath>

// How long (in seconds) between consecutive shots.
static const float SHOOT_INTERVAL = 0.3f;

PlayerTank::PlayerTank(float x, float y)
    : Tank(x, y, sf::Color(80, 200, 80), 3, 200.0f),
      shootCooldown(0.0f)
{
}

void PlayerTank::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    // --- Movement (same as before) ---
    float dx = 0.0f;
    float dy = 0.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    dy -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  dy += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  dx -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) dx += 1.0f;

    float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0.0f)
    {
        dx /= length;
        dy /= length;
        position.x += dx * speed * dt;
        position.y += dy * speed * dt;
        rotation = std::atan2(dy, dx) * 180.0f / 3.14159265f;
    }

    shape.setPosition(position);
    shape.setRotation(sf::degrees(rotation));

    // --- Shooting ---
    // Tick down the cooldown. When it hits 0, shooting becomes available.
    if (shootCooldown > 0.0f)
        shootCooldown -= dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && shootCooldown <= 0.0f)
    {
        // Calculate the bullet direction from the current rotation.
        // We convert the angle (degrees) back to a direction vector.
        float angleRad = rotation * 3.14159265f / 180.0f;
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

        // Spawn the bullet slightly in front of the tank so it doesn't collide with itself.
        float spawnX = position.x + direction.x * 25.0f;
        float spawnY = position.y + direction.y * 25.0f;

        // Add the bullet to the game's object vector.
        // make_unique<Bullet> returns unique_ptr<Bullet>, implicitly convertible
        // to unique_ptr<GameObject> because Bullet inherits from GameObject.
        objects.emplace_back(std::make_unique<Bullet>(spawnX, spawnY, direction, 1));

        // Reset cooldown.
        shootCooldown = SHOOT_INTERVAL;
    }
}