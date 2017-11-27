// player.cpp

#include <iostream>

#include "player.h"

void Player::moveForward(const f32 frameDeltaTime)
{
    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();

    position.X += speed * cos(rotation.Y * M_PI / 180.0f) * frameDeltaTime;
    position.Z += -speed * sin(rotation.Y * M_PI / 180.0f) * frameDeltaTime;

    node->setPosition(position);
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

void Player::setStealth()
{
    if (isFurtive)
        isFurtive = false;
    else
        isFurtive = true;

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
