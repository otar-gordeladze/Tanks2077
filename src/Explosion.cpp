// Explosion.cpp - implementation of the one-shot animated explosion.

#include "Explosion.h"

Explosion::Explosion(float x, float y)
    : GameObject(x, y),
      // Initialize the animation with 6 frames at 0.08 seconds each.
      // Total duration: 6 * 0.08 = 0.48 seconds.
      animation({"explosion_1", "explosion_2", "explosion_3",
                 "explosion_4", "explosion_5", "explosion_6"},
                0.08f, false),
      displaySize(80.0f)
{
    // Create the sprite pointing at the first frame's texture.
    sf::Texture& tex = animation.getCurrentTexture();
    sprite.emplace(tex);

    // Scale to displaySize regardless of source texture size.
    sf::Vector2u texSize = tex.getSize();
    float scale = displaySize / static_cast<float>(texSize.x);
    sprite->setScale(sf::Vector2f(scale, scale));

    // Center origin so position is the middle of the explosion.
    sprite->setOrigin(sf::Vector2f(texSize.x / 2.0f, texSize.y / 2.0f));
    sprite->setPosition(position);
}

void Explosion::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects)
{
    (void)objects;       // explosions don't spawn things

    animation.update(dt);

    // Update the sprite to the current frame's texture.
    if (sprite.has_value())
    {
        sf::Texture& tex = animation.getCurrentTexture();
        sprite->setTexture(tex, true);   // true = reset texture rect to full texture

        // Re-apply scaling and origin because the texture may have a different size.
        sf::Vector2u texSize = tex.getSize();
        float scale = displaySize / static_cast<float>(texSize.x);
        sprite->setScale(sf::Vector2f(scale, scale));
        sprite->setOrigin(sf::Vector2f(texSize.x / 2.0f, texSize.y / 2.0f));
    }

    // When the animation finishes, mark inactive so Game cleans us up.
    if (animation.isFinished())
        active = false;
}

void Explosion::draw(sf::RenderWindow& window)
{
    if (sprite.has_value())
        window.draw(*sprite);
}

sf::FloatRect Explosion::getBounds() const
{
    // Explosions don't collide with anything; return a zero-sized rect.
    return sf::FloatRect(position, sf::Vector2f(0, 0));
}