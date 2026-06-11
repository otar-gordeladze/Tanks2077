// Bonus.h - abstract base class for all bonus pickups.

#pragma once

#include "GameObject.h"
#include <optional>
#include <string>

class PlayerTank;

class Bonus : public GameObject
{
protected:
    std::optional<sf::Sprite> sprite;

    // Helper for subclasses to load and configure their sprite.
    void initSprite(const std::string& textureName);

public:
    Bonus(float x, float y);

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    virtual void apply(PlayerTank& player) = 0;
};