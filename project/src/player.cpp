// player.cpp

#include <iostream>
#include <cmath>

#include "player.h"

void Player::move(const f32 frameDeltaTime, float angle)
{
    ic::vector3df position = node->getPosition();   // recuperation de la position du joueur
    ic::vector3df rotation = node->getRotation();   // recuperation de l orientation du joueur
    rotation.Y = angle * 180.0f/M_PI;               // recalcule de l angle de regard du joueur suivant l angle en parametre

    position.X += speed * std::cos(angle) * frameDeltaTime;  // avance selon l angle en parametre
    position.Z += -speed * std::sin(angle) * frameDeltaTime;

    node->setPosition(position);    // mise a jour de la position du joueur
    node->setRotation(rotation);    // mise a jour de l orientation du joueur
}

void Player::jump(const f32 frameDeltaTime)
{
    ic::vector3df position = node->getPosition();

    position.Y += 500 * frameDeltaTime;

    node->setPosition(position);
}

void Player::setStealth()
{
    isFurtive = !isFurtive;

    if (isFurtive)
    {
        if (isWalking)
            updateAnimation(is::EMAT_CROUCH_WALK);
        else
            updateAnimation(is::EMAT_CROUCH_STAND);
        speed = STEALTH_SPEED;
    }
    else
    {
        if (isWalking)
            updateAnimation(is::EMAT_RUN);
        else
            updateAnimation(is::EMAT_STAND);
        speed = NORMAL_SPEED;
    }
}

void Player::setIdle()
{
    isWalking = false;

    if (isFurtive)
        updateAnimation(is::EMAT_CROUCH_STAND);
    else
        updateAnimation(is::EMAT_STAND);
}

void Player::setWalkAnimation()
{
    isWalking = true;

    if (isFurtive)
        updateAnimation(is::EMAT_CROUCH_WALK);
    else
        updateAnimation(is::EMAT_RUN);
}

std::vector<int> Player::attack(is::ISceneCollisionManager *collMan, const scene::ICameraSceneNode *camera)
{
    std::vector<int> retour = {-1, -1};

    ic::line3d<f32> ray;
    ray.start = camera->getTarget();
    ray.end = ray.start + (camera->getTarget() - camera->getPosition()).normalize() * 1000.0f;

    // Tracks the current intersection point with the level or a mesh
    core::vector3df intersection;
    // Used to show with triangle has been hit
    core::triangle3df hitTriangle;

    // This call is all you need to perform ray/triangle collision on every scene node
    is::ISceneNode * selectedSceneNode =
            collMan->getSceneNodeAndCollisionPointFromRay(
                ray,
                intersection,   // This will be the position of the collision
                hitTriangle,    // This will be the triangle hit in the collision
                0,              // This ensures that only nodes that we have
                // set up to be pickable are considered
                0);             // Check the entire scene (this is actually the implicit default)

    if(selectedSceneNode)
    {
        f64 dist = intersection.getDistanceFrom(camera->getTarget());
        retour[0] = selectedSceneNode->getID();
        retour[1] = dist;
    }
    return retour;
}

bool Player::getHitted()
{
    updateAnimation(is::EMAT_DEATH_FALLBACK);
    return true;
}

void Player::debug(int debug_type)
{
    u32 debug_info = node->isDebugDataVisible();
    node->setDebugDataVisible(debug_info ^ debug_type);
}

std::wstring Player::to_string() const
{
    ic::vector3df p = node->getPosition();
    std::wstring str;
    str = L"Joueur: x[" + std::to_wstring(p.X) + L"], y[" + std::to_wstring(p.Y) + L"], z[" + std::to_wstring(p.Z) + L"]";
    return str;
}
