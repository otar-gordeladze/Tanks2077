// Tank.cpp - implementation of the Tank base class.

#include "Tank.h"
#include "AssetManager.h"

Tank::Tank(float x, float y, int hp, float speed)
    : GameObject(x, y), hp(hp), speed(speed), rotation(0.0f)
{
    // sprite stays empty here - subclasses call initSprite() to populate it.
}

void Tank::initSprite(const std::string& textureName)
{
    // Get the texture from the AssetManager.
    sf::Texture& texture = AssetManager::get().getTexture(textureName);

    // Construct the sprite in place, pointing at this texture.
    sprite.emplace(texture);

    // Tank sprites are large (~1000-1200px from AI). We want them ~50px on screen.
    // Get the actual texture size and compute a uniform scale factor.
    sf::Vector2u texSize = texture.getSize();
    const float TARGET_SIZE = 50.0f;
    float scaleX = TARGET_SIZE / static_cast<float>(texSize.x);
    float scaleY = TARGET_SIZE / static_cast<float>(texSize.y);
    sprite->setScale(sf::Vector2f(scaleX, scaleY));

    // Set origin to the center of the texture so position = center and rotation
    // happens around the center (same trick as the rectangle had).
    sprite->setOrigin(sf::Vector2f(texSize.x / 2.0f, texSize.y / 2.0f));

    sprite->setPosition(position);
}

void Tank::draw(sf::RenderWindow& window)
{
    if (sprite.has_value())
        window.draw(*sprite);
}

sf::FloatRect Tank::getBounds() const
{
    if (sprite.has_value())
        return sprite->getGlobalBounds();
    // Fallback - shouldn't happen if initSprite was called.
    return sf::FloatRect(position, sf::Vector2f(0, 0));
}

void Tank::takeDamage(int amount)
{
    hp -= amount;
    if (hp <= 0)
    {
        hp = 0;
        active = false;
    }
}



















// Tank.cpp - implementation of the Tank base class.

// #include "Tank.h"

// // Tank constructor. Notice the member initializer list:
// // - GameObject(x, y)  -> calls the base class constructor
// // - hp(hp), speed(speed), rotation(0.0f) -> initializes our members
// Tank::Tank(float x, float y, sf::Color color, int hp, float speed)
//     : GameObject(x, y), hp(hp), speed(speed), rotation(0.0f)
// {
//     // Configure the visible rectangle. The tank is 40x40 pixels.
//     shape.setSize(sf::Vector2f(40.0f, 40.0f));
//     shape.setFillColor(color);

//     // Setting the origin to the CENTER of the rectangle (instead of top-left)
//     // means setPosition() places the CENTER at that point, and rotations
//     // happen around the center. This is what you want for a tank.
//     shape.setOrigin(sf::Vector2f(20.0f, 20.0f));

//     // Place the shape at our position.
//     shape.setPosition(position);
// }

// void Tank::draw(sf::RenderWindow& window)
// {
//     // Polymorphism: even though Game stores us as GameObject*, this overridden
//     // draw() runs correctly thanks to the "virtual" keyword in GameObject.
//     window.draw(shape);
// }

// void Tank::takeDamage(int amount)
// {
//     hp -= amount;
//     if (hp <= 0)
//     {
//         hp = 0;
//         active = false;   // Tells Game to remove us next frame.
//     }
// }

// sf::FloatRect Tank::getBounds() const
// {
//     // shape.getGlobalBounds() returns the actual rectangle on screen,
//     // accounting for position, scale, and rotation (approximate axis-aligned box).
//     return shape.getGlobalBounds();
// }