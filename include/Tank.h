// Tank.h - abstract base class for any tank (player or enemy).
// Holds shared data, the visible Sprite, and shared shooting logic.

#pragma once

#include "GameObject.h"
#include <optional>
#include <memory>

class Tank : public GameObject
{
protected:
    int hp;
    float speed;
    float rotation;

    std::optional<sf::Sprite> sprite;

    // --- Shooting infrastructure (shared by all tanks) ---

    // Name of the bullet texture this tank fires (e.g. "player_bullet").
    std::string bulletTextureName;

    // Time remaining until this tank can shoot again.
    float shootCooldown;

    // Time between consecutive shots when at max fire rate.
    float shootInterval;

    void initSprite(const std::string& textureName);
    
    // Whether bullets fired by this tank damage the player (true) or enemies (false).
    bool isEnemyTank;

    // Name of the sound to play when this tank shoots.
    std::string shootSoundName;

    // Factor applied to getBounds(): hitbox is shrunk by this proportion (1.0 = full sprite, 0.7 = 70%).
    float hitboxScale;

public:
    // Constructor now also takes the bullet texture name and base shoot interval.
    Tank(float x, float y, int hp, float speed,
    const std::string& bulletTextureName, float shootInterval,
    bool isEnemyTank, const std::string& shootSoundName);

    virtual ~Tank() = default;

    void draw(sf::RenderWindow& window) override;
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override = 0;
    sf::FloatRect getBounds() const override;

    void takeDamage(int amount);
    int getHP() const { return hp; }

    // Shared shooting logic. Subclasses call this when their logic decides to fire.
    // Spawns a bullet in front of the tank in the current rotation direction.
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects);

    // True if the tank is allowed to fire right now (cooldown has expired).
    bool canShoot() const { return shootCooldown <= 0.0f; }
};