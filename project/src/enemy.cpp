// Enemy.cpp

#include <iostream>

#include <irrlicht.h>

#include "enemy.h"

/// le joueur est touche par un ennemi
bool Enemy::getHitted()
{
    updateAnimation(is::EMAT_DEATH_FALLBACK);
    return true;
}
