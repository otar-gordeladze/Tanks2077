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
      fastMovementTimer(0.0f),
      shieldPulseTime(0.0f)
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
    // --- Shield pulse timer (advances animation) ---
    if (shieldTimer > 0.0f)
        shieldPulseTime += dt;
    else
        shieldPulseTime = 0.0f;

    // --- Visual tint: damage flicker only (shield handled via glow in draw()) ---
    if (sprite.has_value())
    {
        if (damageInvulnerability > 0.0f)
            sprite->setColor(sf::Color(255, 255, 255, 120));     // semi-transparent
        else
            sprite->setColor(sf::Color(255, 255, 255, 255));     // normal
    }
}

bool PlayerTank::collidesWithWall(float testX, float testY,
                                   const std::vector<std::unique_ptr<GameObject>>& objects) const
{
    const float HALF = 70.0f * 0.7f / 2.0f;     // half of hitbox (= 17.5px)
    const float SIZE = 70.0f * 0.7f;             // hitbox size (= 35px)
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


void PlayerTank::draw(sf::RenderWindow& window)
{
    // If shield is active, draw a pulsing cyan glow circle behind the tank.
    if (shieldTimer > 0.0f)
    {
        // Pulse alpha between ~80 and ~220 using a sine wave.
        // sin(time * speed) returns -1..1, we map to 0..1, then scale to alpha range.
        float pulse = (std::sin(shieldPulseTime * 6.0f) + 1.0f) / 2.0f;  // 0..1
        std::uint8_t alpha = static_cast<std::uint8_t>(80 + pulse * 140);  // 80..220

        sf::CircleShape glow(40.0f);    // radius slightly bigger than tank
        glow.setOrigin(sf::Vector2f(40.0f, 40.0f));
        glow.setPosition(position);
        glow.setFillColor(sf::Color(80, 220, 255, alpha));         // cyan
        glow.setOutlineColor(sf::Color(255, 255, 255, alpha));     // white edge
        glow.setOutlineThickness(2.0f);

        window.draw(glow);
    }

    // Then draw the tank sprite normally (inherited behavior).
    Tank::draw(window);
}

void PlayerTank::grantShield(float duration)        { shieldTimer = duration; }
void PlayerTank::grantFastShoot(float duration)     { fastShootTimer = duration; }
void PlayerTank::grantFastMovement(float duration)  { fastMovementTimer = duration; }