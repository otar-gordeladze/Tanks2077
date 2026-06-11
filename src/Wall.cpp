// Wall.cpp - destructible textured wall (tiled).

#include "Wall.h"
#include "AssetManager.h"
#include <algorithm>

Wall::Wall(float x, float y, float w, float h, int hp)
    : GameObject(x, y), hp(hp), maxHp(hp), width(w), height(h)
{
    sf::Texture& texture = AssetManager::get().getTexture("wall");
    texture.setRepeated(true);

    sprite.emplace(texture);

    // Tile the texture instead of stretching: scale + texture rect math.
    const float SCALE = 0.08f;     // adjust if you have a differently-sized wall.png
    sprite->setTextureRect(sf::IntRect(
        sf::Vector2i(0, 0),
        sf::Vector2i(static_cast<int>(w / SCALE),
                     static_cast<int>(h / SCALE))
    ));
    sprite->setScale(sf::Vector2f(SCALE, SCALE));

    sprite->setPosition(position);
}

void Wall::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    (void)dt;
    (void)objects;
}

void Wall::draw(sf::RenderWindow& window)
{
    if (sprite.has_value())
        window.draw(*sprite);
}

sf::FloatRect Wall::getBounds() const
{
    if (sprite.has_value())
        return sprite->getGlobalBounds();
    return sf::FloatRect(position, sf::Vector2f(width, height));
}

void Wall::takeDamage(int amount)
{
    hp -= amount;

    if (sprite.has_value() && maxHp > 0)
    {
        float ratio = static_cast<float>(hp) / static_cast<float>(maxHp);
        std::uint8_t brightness = static_cast<std::uint8_t>(80 + 175 * std::max(0.0f, ratio));
        sprite->setColor(sf::Color(brightness, brightness, brightness, 255));
    }

    if (hp <= 0)
        active = false;
}