// Tank.h - abstract base class for any tank (player or enemy).
// Holds shared data: HP, speed, direction, and the visible shape.
// Subclasses (PlayerTank, EnemyTank) decide HOW to move - they override update().

#pragma once

#include "GameObject.h"

class Tank : public GameObject
{
protected:
    // Current health points. When <= 0, the tank dies (active = false).
    int hp;

    // Movement speed in pixels per second.
    float speed;

    // The visible shape - a colored rectangle. Later replaced with a textured Sprite.
    sf::RectangleShape shape;

    // The direction this tank is facing (angle in degrees, 0 = right, 90 = down).
    float rotation;

public:
    // Constructor: position, color, hp, and speed.
    Tank(float x, float y, sf::Color color, int hp, float speed);

    sf::FloatRect getBounds() const override;

    // Virtual destructor - because Tank is itself a base class (EnemyTank inherits from it).
    virtual ~Tank() = default;

    // We provide draw() here because all tanks draw the same way.
    // Tank is still abstract because update() remains pure virtual.
    void draw(sf::RenderWindow& window) override;

    // update() is still pure virtual - subclasses MUST implement it.
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override = 0;

    // Inflict damage on this tank. If hp drops to 0 or below, mark as inactive.
    void takeDamage(int amount);

    // Getter so other code (UI, scoring) can read hp.
    int getHP() const { return hp; }
};