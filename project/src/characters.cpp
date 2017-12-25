/*!
 * \file characters.cpp
 * \brief Implementation des fonctions de characters.h
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include <iostream>
#include <irrlicht.h>
#include "characters.h"


void Characters::updateAnimation(is::EMD2_ANIMATION_TYPE _animation)
{
    if (animation != _animation)
    {
        animation = _animation;
        if(!isAttacking && !isSuffering)
            node->setMD2Animation(animation);
    }
}

const ic::vector3df& Characters::getPosition() const
{
    return node->getPosition();
}

const ic::vector3df& Characters::getRotation() const
{
    return node->getRotation();
}

void Characters::die()
{
    node->setLoopMode(false);
    node->setAnimationEndCallback(this);
    node->setMD2Animation(is::EMAT_CROUCH_DEATH);
}
