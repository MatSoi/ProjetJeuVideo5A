/*!
 * \file enemy.cp
 * \brief Implementation des fonctions de la classe Enemy
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include <iostream>
#include <irrlicht.h>
#include "enemy.h"

bool Enemy::getHitted()
{
    updateAnimation(is::EMAT_DEATH_FALLBACK);
    return true;
}
