// Wall.h - destructible wall.

#pragma once

#include "GameObject.h"
#include <optional>

class Wall : public GameObject
{
private:
    int hp;
    int maxHp;
    std::optional<sf::Sprite> sprite;
    float width;
    float height;

public:
    Wall(float x, float y, float width, float height, int hp);

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    void takeDamage(int amount);
    int getHP() const { return hp; }
};