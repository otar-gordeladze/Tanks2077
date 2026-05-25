// Wall.cpp - implementation of the destructible wall.

#include "Wall.h"

Wall::Wall(float x, float y, float width, float height, int hp)
    : GameObject(x, y), hp(hp)
{
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(120, 90, 60));      // brown
    shape.setOutlineColor(sf::Color(60, 45, 30));    // darker brown outline
    shape.setOutlineThickness(2.0f);

    // For walls we DON'T center the origin - position = top-left corner,
    // which is the most natural way to place walls on a grid.
    shape.setPosition(position);
}

void Wall::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    // Walls are static - nothing to update.
    // The (void) casts silence "unused parameter" warnings.
    (void)dt;
    (void)objects;
}

void Wall::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Wall::getBounds() const
{
    return shape.getGlobalBounds();
}

void Wall::takeDamage(int amount)
{
    hp -= amount;
    if (hp <= 0)
    {
        active = false;
    }
}