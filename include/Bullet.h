// Bullet.h - a projectile fired by a tank.
// Moves in a straight line, dies when off-screen.

#pragma once

#include "GameObject.h"

class Bullet : public GameObject
{
private:
    // Movement direction (unit vector) and speed in pixels/second.
    sf::Vector2f velocity;

    // How much damage this bullet deals on hit. Used in Phase 3.
    int damage;

    // The visible shape - a small circle.
    sf::CircleShape shape;

public:
    // Constructor: starting position, direction (any vector — we'll normalize), and damage.
    Bullet(float x, float y, sf::Vector2f direction, int damage);

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void draw(sf::RenderWindow& window) override;

    sf::FloatRect getBounds() const override;

    int getDamage() const { return damage; }
};