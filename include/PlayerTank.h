// PlayerTank.h - the tank controlled by the player.

#pragma once

#include "Tank.h"

class PlayerTank : public Tank
{
private:
    // PlayerTank no longer owns shootCooldown - it's inherited from Tank.
    // It DOES own the damage invulnerability + buff timers.

    float damageInvulnerability;
    float shieldTimer;
    float fastShootTimer;
    float fastMovementTimer;
    // Time accumulator for the shield pulse animation.
    float shieldPulseTime;

    bool collidesWithWall(float testX, float testY,
                          const std::vector<std::unique_ptr<GameObject>>& objects) const;

public:
    PlayerTank(float x, float y);
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    
    void draw(sf::RenderWindow& window) override;
    bool tryDamage(int amount);
    void grantShield(float duration);
    void grantFastShoot(float duration);
    void grantFastMovement(float duration);
};