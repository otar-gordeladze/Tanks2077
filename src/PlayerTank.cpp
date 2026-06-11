// PlayerTank.cpp - implementation of the player-controlled tank.

#include "PlayerTank.h"
#include "Wall.h"
#include "Config.h"
#include <cmath>
#include "SoundPlayer.h"


PlayerTank::PlayerTank(float x, float y)
    : Tank(x, y,
           Config::get().getInt("player_hp", 3),
           Config::get().getFloat("player_speed", 200.0f),
           "player_bullet",
           Config::get().getFloat("shoot_cooldown", 0.3f),
           false,            // not an enemy
           "player_shoot"),  // play "player_shoot" sound when firing
      damageInvulnerability(0.0f),
      shieldTimer(0.0f),
      fastShootTimer(0.0f),
      fastMovementTimer(0.0f)
{
    initSprite("player_tank");
}

void PlayerTank::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    // --- Tick down all timers ---
    if (damageInvulnerability > 0.0f) damageInvulnerability -= dt;
    if (shieldTimer > 0.0f)           shieldTimer -= dt;
    if (fastShootTimer > 0.0f)        fastShootTimer -= dt;
    if (fastMovementTimer > 0.0f)     fastMovementTimer -= dt;
    if (shootCooldown > 0.0f)         shootCooldown -= dt;     // inherited from Tank

    // Apply fast-shoot buff by overriding the shoot interval temporarily.
    shootInterval = (fastShootTimer > 0.0f)
        ? Config::get().getFloat("fast_shoot_cooldown", 0.12f)
        : Config::get().getFloat("shoot_cooldown", 0.3f);

    // --- Read input and move ---
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

        float effectiveSpeed = (fastMovementTimer > 0.0f)
            ? speed * Config::get().getFloat("fast_movement_multiplier", 1.5f)
            : speed;

        float newX = position.x + dx * effectiveSpeed * dt;
        if (!collidesWithWall(newX, position.y, objects)) position.x = newX;

        float newY = position.y + dy * effectiveSpeed * dt;
        if (!collidesWithWall(position.x, newY, objects)) position.y = newY;

        rotation = std::atan2(dy, dx) * 180.0f / 3.14159265f;
    }

    if (sprite.has_value())
    {
        sprite->setPosition(position);
        sprite->setRotation(sf::degrees(rotation));
    }

    // --- Shooting: just call the shared method when Space is pressed ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        shoot(objects);     // inherited from Tank; handles cooldown internally

    // --- Visual tint for buffs / damage flicker ---
    if (sprite.has_value())
    {
        if (shieldTimer > 0.0f)
            sprite->setColor(sf::Color(150, 200, 255, 255));
        else if (damageInvulnerability > 0.0f)
            sprite->setColor(sf::Color(255, 255, 255, 120));
        else
            sprite->setColor(sf::Color(255, 255, 255, 255));
    }
}

bool PlayerTank::collidesWithWall(float testX, float testY,
                                   const std::vector<std::unique_ptr<GameObject>>& objects) const
{
    const float HALF = 50.0f * 0.7f / 2.0f;     // half of hitbox (= 17.5px)
    const float SIZE = 50.0f * 0.7f;             // hitbox size (= 35px)
    sf::FloatRect testBounds(sf::Vector2f(testX - HALF, testY - HALF),
                          sf::Vector2f(SIZE, SIZE));

    for (const auto& obj : objects)
    {
        const Wall* wall = dynamic_cast<const Wall*>(obj.get());
        if (wall == nullptr) continue;

        if (testBounds.findIntersection(wall->getBounds()).has_value())
            return true;
    }
    return false;
}

bool PlayerTank::tryDamage(int amount)
{
    if (shieldTimer > 0.0f) return false;
    if (damageInvulnerability > 0.0f) return false;

    takeDamage(amount);
    damageInvulnerability = 1.0f;
    SoundPlayer::get().play("hit");
    return true;
}

void PlayerTank::grantShield(float duration)        { shieldTimer = duration; }
void PlayerTank::grantFastShoot(float duration)     { fastShootTimer = duration; }
void PlayerTank::grantFastMovement(float duration)  { fastMovementTimer = duration; }