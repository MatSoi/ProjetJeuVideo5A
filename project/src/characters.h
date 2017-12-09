// characters.h

#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <vector>

#include <irrlicht.h>

#define NORMAL_SPEED 150.0f
#define STEALTH_SPEED 100.0f
#define ENEMY_SPEED 80.0f

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

// classe abstraite contenant les fonctions et variables communes au personnage principal et aux ennemis
class Characters
{  
public:
    Characters () {}

    /// constructeur
    Characters(is::IAnimatedMeshSceneNode* _node, is::EMD2_ANIMATION_TYPE _animation, float _speed)
        : node(_node), animation(_animation), speed(_speed) {node->setMD2Animation(animation);}

    /// destructeur virtual
    virtual ~Characters() {}

    /// mise a jour de l animation si celle ci est differente
    void updateAnimation (is::EMD2_ANIMATION_TYPE _animation);

    /// assesseur de position
    const ic::vector3df &getPosition();

    /// assesseur d orientation
    const ic::vector3df &getRotation();

    /// fonction de prise de degat
    virtual bool getHitted() = 0;

protected:
    is::IAnimatedMeshSceneNode* node;
    is::EMD2_ANIMATION_TYPE animation;
    float speed;
};

#endif
