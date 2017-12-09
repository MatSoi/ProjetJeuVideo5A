// enemy.h

#ifndef ENEMY_H
#define ENEMY_H

#include <vector>

#include <irrlicht.h>

#include "characters.h"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

class Enemy : public Characters
{
public:
    Enemy () {}

    /// constructeur avec heritage
    Enemy (is::IAnimatedMeshSceneNode* _node, is::EMD2_ANIMATION_TYPE _animation = is::EMAT_STAND, float _speed = NORMAL_SPEED)
        : Characters (_node, _animation, _speed) {}

    /// l enemi est touche par le joueur
    bool getHitted();
};

#endif
