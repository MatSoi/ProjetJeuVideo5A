/*!
 * \file enemy.cpp
 * \brief Implementation des fonctions de la classe Enemy
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include <iostream>
#include <irrlicht.h>
#include "enemy.h"

bool Enemy::getHitted()
{
    node->setLoopMode(false);
    node->setAnimationEndCallback(this);
    updateAnimation(is::EMAT_CROUCH_DEATH);
    return true;
}

void Enemy::OnAnimationEnd(is::IAnimatedMeshSceneNode *node) {
    node->setVisible(false);
    node->setPosition(ic::vector3df(0.0f, -1000.0f, 0.0f));
}
