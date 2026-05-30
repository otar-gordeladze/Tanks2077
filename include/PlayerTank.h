// PlayerTank.h - the tank controlled by the player.
// Inherits from Tank; the only thing it overrides is update() to read keyboard input.

#pragma once
#include "Bullet.h"
#include "Tank.h"

class PlayerTank : public Tank
{
private:
    // Time remaining until the player can fire again. Counts down each frame.
    // Returns true if a 40x40 box at (testX, testY) overlaps any Wall in objects.
    bool collidesWithWall(float testX, float testY,
                      const std::vector<std::unique_ptr<GameObject>>& objects) const;
    float shootCooldown;
    // Brief invulnerability after taking damage. Counts down each frame.
    float damageInvulnerability;

public:
    
    // Constructor: just position. Color, hp, speed are decided by PlayerTank itself.
    PlayerTank(float x, float y);
    
    // Try to damage the player. Returns true if damage was actually applied
    // (false if currently invulnerable).
    bool tryDamage(int amount);

    // Override update() - the abstract method we MUST implement to be concrete.
    // Reads WASD/arrows and moves the tank accordingly.
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
};