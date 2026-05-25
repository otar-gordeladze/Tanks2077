// Wall.h - a destructible wall that blocks movement and absorbs bullets.

#pragma once

#include "GameObject.h"

class Wall : public GameObject
{
private:
    // Wall health. Each bullet hit decreases this. When <= 0, wall is destroyed.
    int hp;

    // The visible shape.
    sf::RectangleShape shape;

public:
    // Constructor: position (top-left), width, height, and HP.
    Wall(float x, float y, float width, float height, int hp);

    // Walls don't move or change on their own - but we still need to override these.
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    // Inflict damage on this wall (called by bullets that hit it).
    void takeDamage(int amount);

    int getHP() const { return hp; }
};