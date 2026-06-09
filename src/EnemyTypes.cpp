// EnemyTypes.cpp - the three concrete enemy tank types.
// Each one passes a different texture name to Tank via initSprite().

#include "EnemyTypes.h"

HeavyEnemy::HeavyEnemy(float x, float y)
    : EnemyTank(x, y, 4, 50.0f, 3.0f, 20)
{
    initSprite("heavy_tank");
}

NormalEnemy::NormalEnemy(float x, float y)
    : EnemyTank(x, y, 2, 80.0f, 2.0f, 10)
{
    initSprite("normal_tank");
}

FastEnemy::FastEnemy(float x, float y)
    : EnemyTank(x, y, 1, 140.0f, 1.0f, 15)
{
    initSprite("fast_tank");
}








// // EnemyTypes.cpp - the three enemy variants. Each just calls EnemyTank's
// // constructor with different stats. This is INHERITANCE for code reuse:
// // almost no new code, but three distinct behaviors.

// #include "EnemyTypes.h"

// // HeavyEnemy: slow, tanky.
// // hp=4, speed=50, direction change every 3 seconds.
// HeavyEnemy::HeavyEnemy(float x, float y)
//     : EnemyTank(x, y, sf::Color(140, 50, 50), 4, 50.0f, 3.0f, 20)   // 20 points
// {
// }

// // NormalEnemy: balanced.
// // hp=2, speed=80, direction change every 2 seconds.
// NormalEnemy::NormalEnemy(float x, float y)
//     : EnemyTank(x, y, sf::Color(220, 130, 50), 2, 80.0f, 2.0f, 10)  // 10 points
// {
// }

// // FastEnemy: glass cannon - quick but dies fast.
// // hp=1, speed=140, direction change every 1 second.
// FastEnemy::FastEnemy(float x, float y)
//     : EnemyTank(x, y, sf::Color(220, 100, 180), 1, 140.0f, 1.0f, 15) // 15 points
// {
// }