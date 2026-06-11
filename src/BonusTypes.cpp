// BonusTypes.cpp - implementation of the three bonus pickups.

#include "BonusTypes.h"
#include "PlayerTank.h"
#include "Config.h"

// --- ShieldBonus ---
ShieldBonus::ShieldBonus(float x, float y)
    : Bonus(x, y)
{
    initSprite("bonus_shield");
}

void ShieldBonus::apply(PlayerTank& player)
{
    player.grantShield(Config::get().getFloat("shield_duration", 5.0f));
}

// --- FastShootBonus ---
FastShootBonus::FastShootBonus(float x, float y)
    : Bonus(x, y)
{
    initSprite("bonus_fast_shoot");
}

void FastShootBonus::apply(PlayerTank& player)
{
    player.grantFastShoot(Config::get().getFloat("fast_shoot_duration", 10.0f));
}

// --- FastMovementBonus ---
FastMovementBonus::FastMovementBonus(float x, float y)
    : Bonus(x, y)
{
    initSprite("bonus_fast_movement");
}

void FastMovementBonus::apply(PlayerTank& player)
{
    player.grantFastMovement(Config::get().getFloat("fast_movement_duration", 10.0f));
}