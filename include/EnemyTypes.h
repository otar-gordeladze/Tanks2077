// EnemyTypes.h - the three concrete enemy tank variants.

#pragma once

#include "EnemyTank.h"

// Heavy tank: slow, tough, dark red.
class HeavyEnemy : public EnemyTank
{
public:
    HeavyEnemy(float x, float y);
};

// Normal tank: medium everything, orange.
class NormalEnemy : public EnemyTank
{
public:
    NormalEnemy(float x, float y);
};

// Fast tank: quick, fragile, pink.
class FastEnemy : public EnemyTank
{
public:
    FastEnemy(float x, float y);
};