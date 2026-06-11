// EnemyTank.h - abstract base class for enemy tanks.

#pragma once

#include "Tank.h"

class EnemyTank : public Tank
{
protected:
    sf::Vector2f direction;
    float directionChangeInterval;
    float directionChangeTimer;
    int scoreValue;

    // AI shoot timer: how long until next shot attempt.
    float shootTimer;

    // Average interval between AI shots (seconds).
    float aiShootInterval;

    void pickRandomDirection();

public:
    EnemyTank(float x, float y, int hp, float speed,
              float changeInterval, int scoreValue,
              const std::string& bulletTextureName,
              float aiShootInterval);

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;

    int getScoreValue() const { return scoreValue; }
};