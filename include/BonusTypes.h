// BonusTypes.h - the three concrete bonus pickups.

#pragma once

#include "Bonus.h"

class ShieldBonus : public Bonus
{
public:
    ShieldBonus(float x, float y);
    void apply(PlayerTank& player) override;
};

class FastShootBonus : public Bonus
{
public:
    FastShootBonus(float x, float y);
    void apply(PlayerTank& player) override;
};

class FastMovementBonus : public Bonus
{
public:
    FastMovementBonus(float x, float y);
    void apply(PlayerTank& player) override;
};