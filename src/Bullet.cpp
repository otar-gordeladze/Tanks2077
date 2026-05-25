// Bullet.cpp - implementation of the projectile.

#include "Bullet.h"
#include <cmath>

// Bullets travel at a fixed speed (in pixels per second).
static const float BULLET_SPEED = 500.0f;

Bullet::Bullet(float x, float y, sf::Vector2f direction, int damage)
    : GameObject(x, y), damage(damage)
{
    // Normalize the direction vector and scale by speed.
    // (We accept ANY direction vector and make it a velocity.)
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f)
    {
        velocity.x = (direction.x / length) * BULLET_SPEED;
        velocity.y = (direction.y / length) * BULLET_SPEED;
    }

    // Configure the visible shape - a small yellow circle, radius 5 pixels.
    shape.setRadius(5.0f);
    shape.setFillColor(sf::Color(255, 220, 80));
    // Origin at center so position = center of the bullet.
    shape.setOrigin(sf::Vector2f(5.0f, 5.0f));
    shape.setPosition(position);
}

void Bullet::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    // We don't spawn other objects, so we ignore the 'objects' parameter.
    // The (void) cast tells the compiler "yes, I know it's unused, don't warn me".
    (void)objects;

    // Move by velocity * dt (frame-independent).
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    shape.setPosition(position);

    // Mark as inactive if off-screen. Game will clean it up.
    if (position.x < 0 || position.x > 800 || position.y < 0 || position.y > 600)
    {
        active = false;
    }
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const
{
    return shape.getGlobalBounds();
}