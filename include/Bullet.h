// Bullet.h - a projectile fired by a tank.

#pragma once

#include "GameObject.h"
#include <optional>
#include <string>

class Bullet : public GameObject
{
private:
    sf::Vector2f velocity;
    int damage;
    std::optional<sf::Sprite> sprite;
    // True if this bullet was fired by an enemy (it can damage player).
    // False if fired by player (it can damage enemies/walls).
    bool fromEnemy;

public:
    // Constructor now takes a textureName so different shooters can use different bullet sprites.
    Bullet(float x, float y, sf::Vector2f direction, int damage,
       const std::string& textureName, bool fromEnemy);
    bool isFromEnemy() const { return fromEnemy; }

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    int getDamage() const { return damage; }
};







// // Bullet.h - a projectile fired by a tank.
// // Moves in a straight line, dies when off-screen.

// #pragma once

// #include "GameObject.h"

// class Bullet : public GameObject
// {
// private:
//     // Movement direction (unit vector) and speed in pixels/second.
//     sf::Vector2f velocity;

//     // How much damage this bullet deals on hit. Used in Phase 3.
//     int damage;

//     // The visible shape - a small circle.
//     sf::CircleShape shape;

// public:
//     // Constructor: starting position, direction (any vector — we'll normalize), and damage.
//     Bullet(float x, float y, sf::Vector2f direction, int damage);

//     void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
//     void draw(sf::RenderWindow& window) override;

//     sf::FloatRect getBounds() const override;

//     int getDamage() const { return damage; }
// };