// characters.cpp

#include <iostream>

#include <irrlicht.h>

#include "characters.h"


void Characters::updateAnimation(is::EMD2_ANIMATION_TYPE _animation)
{
    if (animation != _animation)
    {
        animation = _animation;
        node->setMD2Animation(animation);
    }
}

const ic::vector3df& Characters::getPosition()
{
    return node->getPosition();
}
