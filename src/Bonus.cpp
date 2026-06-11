// Bonus.cpp - shared implementation for all bonus pickups.

#include "Bonus.h"
#include "AssetManager.h"

Bonus::Bonus(float x, float y)
    : GameObject(x, y)
{
    // sprite stays empty here - subclasses call initSprite() to populate it.
}

void Bonus::initSprite(const std::string& textureName)
{
    sf::Texture& texture = AssetManager::get().getTexture(textureName);
    sprite.emplace(texture);

    sf::Vector2u texSize = texture.getSize();
    const float TARGET_SIZE = 36.0f;
    float scale = TARGET_SIZE / static_cast<float>(texSize.x);
    sprite->setScale(sf::Vector2f(scale, scale));

    // Center origin = position is the middle of the bonus.
    sprite->setOrigin(sf::Vector2f(texSize.x / 2.0f, texSize.y / 2.0f));
    sprite->setPosition(position);
}

void Bonus::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    (void)dt;
    (void)objects;
}

void Bonus::draw(sf::RenderWindow& window)
{
    if (sprite.has_value())
        window.draw(*sprite);
}

sf::FloatRect Bonus::getBounds() const
{
    if (sprite.has_value())
        return sprite->getGlobalBounds();
    return sf::FloatRect(position, sf::Vector2f(0, 0));
}