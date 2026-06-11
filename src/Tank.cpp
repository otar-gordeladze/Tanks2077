// Tank.cpp - implementation of the Tank base class.

#include "Tank.h"
#include "AssetManager.h"
#include "Bullet.h"
#include <cmath>

Tank::Tank(float x, float y, int hp, float speed,
           const std::string& bulletTextureName, float shootInterval,
           bool isEnemyTank)
    : GameObject(x, y),
      hp(hp),
      speed(speed),
      rotation(0.0f),
      bulletTextureName(bulletTextureName),
      shootCooldown(0.0f),
      shootInterval(shootInterval),
      isEnemyTank(isEnemyTank)
{
}

void Tank::initSprite(const std::string& textureName)
{
    sf::Texture& texture = AssetManager::get().getTexture(textureName);
    sprite.emplace(texture);

    sf::Vector2u texSize = texture.getSize();
    const float TARGET_SIZE = 50.0f;
    float scaleX = TARGET_SIZE / static_cast<float>(texSize.x);
    float scaleY = TARGET_SIZE / static_cast<float>(texSize.y);
    sprite->setScale(sf::Vector2f(scaleX, scaleY));

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

void Tank::shoot(std::vector<std::unique_ptr<GameObject>>& objects)
{
    // Guard: if still on cooldown, do nothing.
    if (shootCooldown > 0.0f) return;

    // Compute the firing direction from rotation (degrees -> radians -> unit vector).
    float angleRad = rotation * 3.14159265f / 180.0f;
    sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

    // Spawn the bullet in front of the tank so it doesn't immediately self-collide.
    float spawnX = position.x + direction.x * 25.0f;
    float spawnY = position.y + direction.y * 25.0f;

    // Add to the polymorphic vector. The bullet's texture name was set at construction.
    objects.emplace_back(
        std::make_unique<Bullet>(spawnX, spawnY, direction, 1, bulletTextureName, isEnemyTank));

    // Reset cooldown to interval (subclasses may override the interval later).
    shootCooldown = shootInterval;
}