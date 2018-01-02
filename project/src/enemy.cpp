/*!
 * \file enemy.cpp
 * \brief Implementation des fonctions de la classe Enemy
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include "enemy.h"

void Enemy::getHitted()
{
    if(isAlerted)
    {
        life -= 1;
    }
    else
        life = 0;

    if (life)
        pain();
    else
        die();
}

void Enemy::pain()
{
    isSuffering = true;
    node->setLoopMode(false);
    node->setAnimationEndCallback(this);
    node->setMD2Animation(is::EMAT_PAIN_A);
}

void Enemy::die()
{
    node->setLoopMode(false);
    node->setAnimationEndCallback(this);
    node->setMD2Animation(is::EMAT_CROUCH_DEATH);
}

void Enemy::run()
{
    updateAnimation(is::EMAT_RUN);
}

void Enemy::idle()
{
    updateAnimation(is::EMAT_STAND);
}

void Enemy::resetRotation()
{
    node->setRotation(initialRotation);//Reset de la rotation de l'enemie sur la rotation initiale
}

bool Enemy::canAttack()
{
    if(isAttacking)
        return false;

    if (((life == 1) || (distWithPlayer <= ATTACK_DIST_ENEMY && !isSuffering)) && isAlerted)
    {
        attack();
        return true;
    }

    return false;
}

void Enemy::attack()
{
    isAttacking = true;
    node->setLoopMode(false);
    node->setAnimationEndCallback(this);
    node->setMD2Animation(is::EMAT_ATTACK);
}

void Enemy::OnAnimationEnd(is::IAnimatedMeshSceneNode *node)
{
    if(life)
    {
        node->setLoopMode(true);
        node->setMD2Animation(animation);
    }
    else
        node->setVisible(false);

    isSuffering = false;
    isAttacking = false;
}

bool Enemy::isPlayerInEnemyView(const ic::vector3df& playerPosition, irr::scene::ISceneCollisionManager *collMan)
{
    distWithPlayer = playerPosition.getDistanceFrom(getPosition());
    if(distWithPlayer < rayonDetection)//Si le joueur se trouve dans le cercle  => check dans le cone de vision de l'enemi
    {
        ic::vector3df enemyPos = node->getPosition();
        ic::vector3df enemyRot = node->getRotation();

        //On définit deux points correspondants aux extremites de l'angle de vision ennemi
        ic::vector2df pointGauche = ic::vector2df(enemyPos.X, enemyPos.Z);
        ic::vector2df pointDroite = ic::vector2df(enemyPos.X, enemyPos.Z);
        ic::vector2df pointMilieu = ic::vector2df(enemyPos.X, enemyPos.Z);

        pointGauche.X += std::cos((enemyRot.Y - angleViewEnemy) * M_PI / 180.0f);
        pointGauche.Y -= std::sin((enemyRot.Y - angleViewEnemy) * M_PI / 180.0f);

        pointDroite.X += std::cos((enemyRot.Y + angleViewEnemy) * M_PI / 180.0f);
        pointDroite.Y -= std::sin((enemyRot.Y + angleViewEnemy) * M_PI / 180.0f);

        pointMilieu.X += std::cos((enemyRot.Y) * M_PI / 180.0f);
        pointMilieu.Y -= std::sin((enemyRot.Y) * M_PI / 180.0f);

        ic::vector2df vecGauche = ic::vector2df(pointGauche.X - enemyPos.X, pointGauche.Y - enemyPos.Z);
        ic::vector2df vecDroite = ic::vector2df(pointDroite.X - enemyPos.X, pointDroite.Y - enemyPos.Z);
        ic::vector2df vecMilieu = ic::vector2df(pointMilieu.X - enemyPos.X, pointMilieu.Y - enemyPos.Z);
        ic::vector2df vecPlayer = ic::vector2df(playerPosition.X - enemyPos.X, playerPosition.Z - enemyPos.Z);

        if(vecMilieu.dotProduct(vecPlayer) > 0)//Permet de s'assurer que le joueur est devant l'ennemi et non derriere
        {
            float determinantGauche = vecGauche.X * vecPlayer.Y - vecGauche.Y * vecPlayer.X;
            float determinantDroit = vecDroite.X * vecPlayer.Y - vecDroite.Y * vecPlayer.X;

            bool signeGauche = determinantGauche >= 0;
            bool signeDroit = determinantDroit >= 0;

            if((signeGauche && !signeDroit) || (!signeGauche && signeDroit))//Verification que les signes sont opposés => donc le joueur est dans le cone
            {
                //Raycast de l'ennemi vers le joueur et verification de hit
                ic::line3d<irr::f32> ray;
                ray.start = enemyPos;
                ray.end = ray.start + (playerPosition - ray.start).normalize() * RAY_LENGTH;


                ic::vector3df intersection;
                ic::triangle3df hitTriangle;

                is::ISceneNode * selectedSceneNode =
                        collMan->getSceneNodeAndCollisionPointFromRay(
                            ray,
                            intersection,
                            hitTriangle,
                            ID_PLAYER | ID_MAP,
                            0);
                if(selectedSceneNode)
                    if(selectedSceneNode->getID() == ID_PLAYER)
                    {
                        isAlerted = true;
                        return true;
                    }
            }
        }
    }
    isAlerted = false;
    return false;
}

const int& Enemy::getAngleViewEnemy() const
{
    return angleViewEnemy;
}

void Enemy::followPlayer(ic::vector3df playerPosition, const irr::f32 frameDeltaTime, int sizePositions)
{
    ic::vector3df direction = playerPosition - getPosition();
    //Correct enemy rotation according to the direction
    float angleDirection = direction.getHorizontalAngle().Y;
    node->setRotation(ic::vector3df(0,angleDirection - 90,0));

    if(distWithPlayer > ATTACK_DIST_ENEMY - 5)
    {
        //Bouge l'ennemi vers le joueur
        node->setPosition(node->getPosition() + frameDeltaTime * direction);
        //Affecte une animation de course pour l'enemie
        run();

        //Si la distance entre la position de l'ennemi et la derniere enregistre, on enregistre position actuelle
        if(getPosition().getDistanceFrom(positions[sizePositions])>100.0f)
            positions.push_back(getPosition());
    }
    else
        idle();
}

void Enemy::getBackToOriginalPosition(const irr::f32 frameDeltaTime, int sizePositions)
{
    if(sizePositions != 0  && (getPosition().getDistanceFrom(positions[sizePositions])<30.0f))//Si derniere position enregistre est rejoint et qu'il ne s'agit pas de la premiere position, on la supprime et on continue
    {
        positions.erase(sizePositions);
        sizePositions--;
    }

    ic::vector3df direction = positions[sizePositions] - getPosition();
    //Ajuste la direction de l'enemi avec la direction
    float angleDirection = direction.getHorizontalAngle().Y;
    node->setRotation(ic::vector3df(0,angleDirection - 90,0));

    //Avance l'enemi vers la position anterieur
    node->setPosition(node->getPosition() + frameDeltaTime * direction);

    //Animation de run
    run();
}

bool Enemy::normalBehaviour(ic::vector3df playerPosition, const irr::f32 frameDeltaTime, is::ISceneCollisionManager* collMan)
{
    if (!life)
        return false;

    int sizePositions = positions.size()-1;

    if(isPlayerInEnemyView(playerPosition,collMan))//Si je joueur est visible par l'ennemie
        followPlayer(playerPosition, frameDeltaTime, sizePositions);
    else
    {
        if(getPosition().getDistanceFrom(positions[0]) > 30)//Si enemie n'est pas sur sa position initiale
        {
            getBackToOriginalPosition (frameDeltaTime, sizePositions);
        }
        else //Si rien a faire donc sur sa position de départ
            //Set animation to idle et reset rotation
        {
            idle();
            resetRotation();
        }
    }

    return canAttack();
}
