// player.h

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include <irrlicht.h>

#include "characters.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

// classe correspondant au personnage principale
class Player : public Characters
{  
public:
    /// constructeur avec heritage
    Player (is::IAnimatedMeshSceneNode* _node, is::EMD2_ANIMATION_TYPE _animation = is::EMAT_STAND, float _speed = NORMAL_SPEED)
        : Characters (_node, _animation, _speed) {}

    /// destructeur
    virtual ~Player () {}

    /// le joueur avance
    void moveForward (const f32 frameDeltaTime);

    /// le joueur recule
    void moveBackward (const f32 frameDeltaTime);

    /// le joueur tourne
    void rotate (const f32 frameDeltaTime, float angle);

    /// passe ou quitte le mode furtif
    void setStealth ();

    /// remet l animation d immobilite
    void setIdle ();

    /// choisi l animation de marche
    void setWalkAnimation ();

    /// fonction de debub du node
    void debug (int debug_type);

private:
    bool isFurtive = false;
    bool isWalking = false;
};

#endif
