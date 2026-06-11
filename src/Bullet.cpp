// Bullet.cpp - implementation of the projectile.

#include "Bullet.h"
#include "AssetManager.h"
#include "Config.h"
#include <cmath>

Bullet::Bullet(float x, float y, sf::Vector2f direction, int damage,
               const std::string& textureName, bool fromEnemy)
    : GameObject(x, y), damage(damage), fromEnemy(fromEnemy)

{
    float bulletSpeed = Config::get().getFloat("bullet_speed", 500.0f);

    // Normalize direction and scale by speed.
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f)
    {
        velocity.x = (direction.x / length) * bulletSpeed;
        velocity.y = (direction.y / length) * bulletSpeed;
    }

    // Load and configure the sprite.
    sf::Texture& texture = AssetManager::get().getTexture(textureName);
    sprite.emplace(texture);

    sf::Vector2u texSize = texture.getSize();
    const float TARGET_SIZE = 22.0f;     // small bullets on screen
    float scale = TARGET_SIZE / static_cast<float>(texSize.x);
    sprite->setScale(sf::Vector2f(scale, scale));
    sprite->setOrigin(sf::Vector2f(texSize.x / 2.0f, texSize.y / 2.0f));
    sprite->setPosition(position);

    // Rotate the bullet sprite to match its travel direction.
    float angleRad = std::atan2(direction.y, direction.x);
    float angleDeg = angleRad * 180.0f / 3.14159265f;
    sprite->setRotation(sf::degrees(angleDeg));
}

void Bullet::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    (void)objects;

    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    if (sprite.has_value())
        sprite->setPosition(position);

    if (position.x < 0 || position.x > 800 || position.y < 0 || position.y > 600)
        active = false;
}

void Bullet::draw(sf::RenderWindow& window)
{
    if (sprite.has_value())
        window.draw(*sprite);
}

sf::FloatRect Bullet::getBounds() const
{
    if (sprite.has_value())
        return sprite->getGlobalBounds();
    return sf::FloatRect(position, sf::Vector2f(0, 0));
}









// // Bullet.cpp - implementation of the projectile.
// #include "Bullet.h"
// #include <cmath>
// #include "Config.h"




// // Bullets travel at a fixed speed (in pixels per second).
// // static const float BULLET_SPEED = 500.0f;

// Bullet::Bullet(float x, float y, sf::Vector2f direction, int damage)
//     : GameObject(x, y), damage(damage)
// {
//     // Read bullet speed from config (with sensible fallback).
//     float bulletSpeed = Config::get().getFloat("bullet_speed", 500.0f);

//     // Normalize the direction vector and scale by speed.
//     float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
//     if (length > 0.0f)
//     {
//         velocity.x = (direction.x / length) * bulletSpeed;
//         velocity.y = (direction.y / length) * bulletSpeed;
//     }

//     // Configure the visible shape - a small yellow circle, radius 5 pixels.
//     shape.setRadius(5.0f);
//     shape.setFillColor(sf::Color(255, 220, 80));
//     // Origin at center so position = center of the bullet.
//     shape.setOrigin(sf::Vector2f(5.0f, 5.0f));
//     shape.setPosition(position);
// }

// void Bullet::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
// {
//     // We don't spawn other objects, so we ignore the 'objects' parameter.
//     // The (void) cast tells the compiler "yes, I know it's unused, don't warn me".
//     (void)objects;

//     // Move by velocity * dt (frame-independent).
//     position.x += velocity.x * dt;
//     position.y += velocity.y * dt;
//     shape.setPosition(position);

//     // Mark as inactive if off-screen. Game will clean it up.
//     if (position.x < 0 || position.x > 800 || position.y < 0 || position.y > 600)
//     {
//         active = false;
//     }
// }

// void Bullet::draw(sf::RenderWindow& window)
// {
//     window.draw(shape);
// }

// sf::FloatRect Bullet::getBounds() const
// {
//     return shape.getGlobalBounds();
// }