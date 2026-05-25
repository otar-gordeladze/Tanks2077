// Tank.cpp - implementation of the Tank base class.

#include "Tank.h"

// Tank constructor. Notice the member initializer list:
// - GameObject(x, y)  -> calls the base class constructor
// - hp(hp), speed(speed), rotation(0.0f) -> initializes our members
Tank::Tank(float x, float y, sf::Color color, int hp, float speed)
    : GameObject(x, y), hp(hp), speed(speed), rotation(0.0f)
{
    // Configure the visible rectangle. The tank is 40x40 pixels.
    shape.setSize(sf::Vector2f(40.0f, 40.0f));
    shape.setFillColor(color);

    // Setting the origin to the CENTER of the rectangle (instead of top-left)
    // means setPosition() places the CENTER at that point, and rotations
    // happen around the center. This is what you want for a tank.
    shape.setOrigin(sf::Vector2f(20.0f, 20.0f));

    // Place the shape at our position.
    shape.setPosition(position);
}

void Tank::draw(sf::RenderWindow& window)
{
    // Polymorphism: even though Game stores us as GameObject*, this overridden
    // draw() runs correctly thanks to the "virtual" keyword in GameObject.
    window.draw(shape);
}

void Tank::takeDamage(int amount)
{
    hp -= amount;
    if (hp <= 0)
    {
        hp = 0;
        active = false;   // Tells Game to remove us next frame.
    }
}

sf::FloatRect Tank::getBounds() const
{
    // shape.getGlobalBounds() returns the actual rectangle on screen,
    // accounting for position, scale, and rotation (approximate axis-aligned box).
    return shape.getGlobalBounds();
}