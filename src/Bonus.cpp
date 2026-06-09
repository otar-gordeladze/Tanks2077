// Bonus.cpp - shared implementation for all bonus pickups.

#include "Bonus.h"

Bonus::Bonus(float x, float y, sf::Color color)
    : GameObject(x, y)
{
    // All bonuses are 24x24 colored squares with a white outline.
    shape.setSize(sf::Vector2f(24.0f, 24.0f));
    shape.setOrigin(sf::Vector2f(12.0f, 12.0f));   // center origin
    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.0f);
    shape.setPosition(position);
}

void Bonus::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    // Bonuses are passive - they don't move or change.
    (void)dt;
    (void)objects;
}

void Bonus::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Bonus::getBounds() const
{
    return shape.getGlobalBounds();
}