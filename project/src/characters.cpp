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

int Characters::getLife() const
{
    return life;
}

bool Characters::isDead()
{
    if(node->getPosition().Y < END_MAP_Y)
    {
        die();
        life = 0;
    }

    return life == 0;
}
