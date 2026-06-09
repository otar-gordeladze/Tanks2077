// Bonus.h - abstract base class for all bonus pickups.
// Three subclasses (Shield, FastShoot, FastMovement) implement apply() differently.

#pragma once

#include "GameObject.h"

class PlayerTank;     // forward declaration - avoids circular #include

class Bonus : public GameObject
{
protected:
    sf::RectangleShape shape;

public:
    // Constructor: position and color.
    Bonus(float x, float y, sf::Color color);

    // Bonuses don't move; they just exist until picked up or removed.
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    // THE polymorphic method - subclasses define what the bonus actually does.
    virtual void apply(PlayerTank& player) = 0;
};