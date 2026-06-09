// BonusTypes.cpp - implementation of the three bonus pickups.
// Each one calls a different player method via apply().
#include "BonusTypes.h"
#include "PlayerTank.h"   // we DO need the full definition here, since we call methods on PlayerTank
#include "Config.h"




// --- ShieldBonus ---
// Cyan square. Grants 5 seconds of invulnerability.
ShieldBonus::ShieldBonus(float x, float y)
    : Bonus(x, y, sf::Color(80, 180, 255))
{
}

void ShieldBonus::apply(PlayerTank& player)
{
    player.grantShield(Config::get().getFloat("shield_duration", 5.0f));
}

// --- FastShootBonus ---
// Red square. Reduces shoot cooldown for 10 seconds.
FastShootBonus::FastShootBonus(float x, float y)
    : Bonus(x, y, sf::Color(230, 70, 70))
{
}

void FastShootBonus::apply(PlayerTank& player)
{
    player.grantFastShoot(Config::get().getFloat("fast_shoot_duration", 10.0f));
}

// --- FastMovementBonus ---
// Yellow square. +50% movement speed for 10 seconds.
FastMovementBonus::FastMovementBonus(float x, float y)
    : Bonus(x, y, sf::Color(240, 220, 60))
{
}

void FastMovementBonus::apply(PlayerTank& player)
{
    player.grantFastMovement(Config::get().getFloat("fast_movement_duration", 10.0f));
}