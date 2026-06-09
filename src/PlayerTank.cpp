// PlayerTank.cpp - implementation of the player-controlled tank.

#include "PlayerTank.h"
#include "Bullet.h"
#include <cmath>
#include "Wall.h"
#include "Config.h"


// How long (in seconds) between consecutive shots.
static const float SHOOT_INTERVAL = 0.3f;

PlayerTank::PlayerTank(float x, float y)
    : Tank(x, y,
           Config::get().getInt("player_hp", 3),
           Config::get().getFloat("player_speed", 200.0f)),
      shootCooldown(0.0f),
      damageInvulnerability(0.0f),
      shieldTimer(0.0f),
      fastShootTimer(0.0f),
      fastMovementTimer(0.0f)
{
    // Load the player tank texture.
    initSprite("player_tank");
}

void PlayerTank::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    // Tick down damage invulnerability
    if (damageInvulnerability > 0.0f)
        damageInvulnerability -= dt;

    // Tick down buff timers
    if (shieldTimer > 0.0f)        shieldTimer -= dt;
    if (fastShootTimer > 0.0f)     fastShootTimer -= dt;
    if (fastMovementTimer > 0.0f)  fastMovementTimer -= dt;

    // --- 1. Read input ---
    float dx = 0.0f, dy = 0.0f;
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

        // Apply movement speed buff if active.
        float effectiveSpeed = (fastMovementTimer > 0.0f)
            ? speed * Config::get().getFloat("fast_movement_multiplier", 1.5f)
            : speed;

        // Try X movement first
        float newX = position.x + dx * effectiveSpeed * dt;
        if (!collidesWithWall(newX, position.y, objects))
            position.x = newX;

        // Then Y movement
        float newY = position.y + dy * effectiveSpeed * dt;
        if (!collidesWithWall(position.x, newY, objects))
            position.y = newY;

        // Always update rotation to face movement direction
        rotation = std::atan2(dy, dx) * 180.0f / 3.14159265f;
    }

    if (sprite.has_value())
    {
        sprite->setPosition(position);
        sprite->setRotation(sf::degrees(rotation));
    }

    // --- 3. Shooting (unchanged) ---
    if (shootCooldown > 0.0f)
        shootCooldown -= dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && shootCooldown <= 0.0f)
    {
        float angleRad = rotation * 3.14159265f / 180.0f;
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));
        float spawnX = position.x + direction.x * 25.0f;
        float spawnY = position.y + direction.y * 25.0f;
        objects.emplace_back(std::make_unique<Bullet>(spawnX, spawnY, direction, 1, "player_bullet"));
        shootCooldown = (fastShootTimer > 0.0f)
            ? Config::get().getFloat("fast_shoot_cooldown", 0.12f)
            : Config::get().getFloat("shoot_cooldown", 0.3f);
    }
    // Visual feedback via sprite color tint.
    // sf::Sprite::setColor() multiplies the texture's pixels by this color.
    // (255,255,255,255) = unchanged. Lower alpha = transparent. Different RGB = tint.
    
    // Visual feedback for active buffs / damage state.
    if (sprite.has_value())
    {
        if (shieldTimer > 0.0f)
            sprite->setColor(sf::Color(150, 200, 255, 255));    // bluish tint = shield
        else if (damageInvulnerability > 0.0f)
            sprite->setColor(sf::Color(255, 255, 255, 120));    // semi-transparent
        else
            sprite->setColor(sf::Color(255, 255, 255, 255));    // normal
    }
}

// Check if moving to (testX, testY) would collide with any Wall in the vector.
// Returns true if blocked.
bool PlayerTank::collidesWithWall(float testX, float testY,
                                   const std::vector<std::unique_ptr<GameObject>>& objects) const
{
    // Build the rectangle the tank WOULD occupy at the test position.
    // The shape's origin is at its center, so subtract half its size.
    sf::FloatRect testBounds(sf::Vector2f(testX - 20.0f, testY - 20.0f),
                              sf::Vector2f(40.0f, 40.0f));

    // Iterate the polymorphic vector and look for Walls.
    for (const auto& obj : objects)
    {
        // dynamic_cast: try to convert GameObject* to Wall*.
        // - If obj actually points to a Wall, returns the Wall* pointer.
        // - If obj is anything else (Tank, Bullet), returns nullptr.
        // This is RUNTIME type checking - the "dynamic" part.
        const Wall* wall = dynamic_cast<const Wall*>(obj.get());
        if (wall == nullptr) continue;     // not a wall - skip

        // Check if our test rectangle intersects this wall.
        // SFML 3.x: findIntersection returns std::optional<FloatRect>.
        if (testBounds.findIntersection(wall->getBounds()).has_value())
            return true;
    }

    return false;
}

bool PlayerTank::tryDamage(int amount)
{
    // Shield blocks damage entirely.
    if (shieldTimer > 0.0f)
        return false;

    if (damageInvulnerability > 0.0f)
        return false;

    takeDamage(amount);
    damageInvulnerability = 1.0f;
    return true;
}

void PlayerTank::grantShield(float duration)        { shieldTimer = duration; }
void PlayerTank::grantFastShoot(float duration)     { fastShootTimer = duration; }
void PlayerTank::grantFastMovement(float duration)  { fastMovementTimer = duration; }