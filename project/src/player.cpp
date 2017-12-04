// player.cpp

#include <iostream>

#include "player.h"

void Player::move(const f32 frameDeltaTime, float angle)
{
    ic::vector3df position = node->getPosition();   // recuperation de la position du joueur
    ic::vector3df rotation = node->getRotation();   // recuperation de l orientation du joueur
    rotation.Y = angle * 180.0f/M_PI;               // recalcule de l angle de regard du joueur suivant l angle en parametre

    position.X += speed * cos(angle) * frameDeltaTime;  // avance selon l angle en parametre
    position.Z += -speed * sin(angle) * frameDeltaTime;

    node->setPosition(position);    // mise a jour de la position du joueur
    node->setRotation(rotation);    // mise a jour de l orientation du joueur
}

void Player::moveBackward(const f32 frameDeltaTime)
{
    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();

    position.X -= (speed - 25.0f) * cos(rotation.Y * M_PI / 180.0f) * frameDeltaTime;
    position.Z -= -(speed - 25.0f) * sin(rotation.Y * M_PI / 180.0f) * frameDeltaTime;

    node->setPosition(position);
}

void Player::rotate(const f32 frameDeltaTime, float angle)
{
    ic::vector3df rotation = node->getRotation();

    rotation.Y += angle * frameDeltaTime;
    node->setRotation(rotation);
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

void Player::debug(int debug_type)
{
    u32 debug_info = node->isDebugDataVisible();
    node->setDebugDataVisible(debug_info ^ debug_type);
}
