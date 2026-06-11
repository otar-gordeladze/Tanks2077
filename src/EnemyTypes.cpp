// EnemyTypes.cpp - the three concrete enemy tank types.
// Each one passes a different sprite texture, bullet texture, and stats.

#include "EnemyTypes.h"

// HEAVY: slow, tanky, fires the orange-red bullet, shoots every ~3.5s.
HeavyEnemy::HeavyEnemy(float x, float y)
    : EnemyTank(x, y, 6, 50.0f, 3.0f, 20, "heavy_bullet", 3.5f)
{
    initSprite("heavy_tank");
}

// NORMAL: balanced, fires the orange bullet, shoots every ~2.5s.
NormalEnemy::NormalEnemy(float x, float y)
    : EnemyTank(x, y, 4, 80.0f, 2.0f, 10, "normal_bullet", 2.5f)
{
    initSprite("normal_tank");
}

// FAST: quick, fragile, fires the magenta bullet, shoots every ~1.8s.
FastEnemy::FastEnemy(float x, float y)
    : EnemyTank(x, y, 3, 140.0f, 1.0f, 15, "fast_bullet", 1.8f)
{
    initSprite("fast_tank");
}