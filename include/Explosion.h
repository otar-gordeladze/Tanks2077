// Explosion.h - a one-shot animated explosion that lives in the game world.
// When the animation finishes, the Explosion marks itself inactive so Game
// removes it from the polymorphic vector. Inherits from GameObject like
// everything else - this is polymorphism doing the work.

#pragma once

#include "GameObject.h"
#include "Animation.h"
#include <optional>

class Explosion : public GameObject
{
private:
    // The animation that drives our visible frame.
    Animation animation;

    // The visible sprite. Re-points to a new texture each frame.
    std::optional<sf::Sprite> sprite;

    // Display size on screen (pixels).
    float displaySize;

public:
    // Constructor: position (center of explosion).
    Explosion(float x, float y);

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
};