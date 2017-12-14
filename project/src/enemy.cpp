/*!
 * \file enemy.cpp
 * \brief Implementation des fonctions de la classe Enemy
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include "enemy.h"

bool Enemy::getHitted()
{
    node->setLoopMode(false);
    node->setAnimationEndCallback(this);
    updateAnimation(is::EMAT_CROUCH_DEATH);
    return true;
}

void Enemy::OnAnimationEnd(is::IAnimatedMeshSceneNode *node) {
    node->setVisible(false);
    node->setPosition(ic::vector3df(0.0f, -1000.0f, 0.0f));
}

bool Enemy::playerIsInEnemyView(ic::vector3df playerPosition, irr::scene::ISceneCollisionManager *collMan)
{
    float dist = sqrt(pow(node->getPosition().X - playerPosition.X,2) + pow(node->getPosition().Z - playerPosition.Z,2));
    if(dist < rayonDetection)//Si le joueur se trouve dans le cercle  => check dans le cone de vision de l'enemi
    {
        ic::vector3df enemyPos= node->getPosition();
        ic::vector3df enemyRot= node->getRotation();

        //On définit deux points correspondants aux extremites de l'angle de vision ennemi
        ic::vector2df pointGauche = ic::vector2df(enemyPos.X, enemyPos.Z);
        ic::vector2df pointDroite = ic::vector2df(enemyPos.X, enemyPos.Z);

        pointGauche.X += std::cos((enemyRot.Y - angleViewEnemy) * M_PI / 180.0f);
        pointGauche.Y -= std::sin((enemyRot.Y + angleViewEnemy) * M_PI / 180.0f);

        pointDroite.X += std::cos((enemyRot.Y + angleViewEnemy) * M_PI / 180.0f);
        pointDroite.Y -= std::sin((enemyRot.Y - angleViewEnemy) * M_PI / 180.0f);

        ic::vector2df vecGauche = ic::vector2df(pointGauche.X - enemyPos.X, pointGauche.Y - enemyPos.Z);
        ic::vector2df vecDroite = ic::vector2df(pointDroite.X - enemyPos.X, pointDroite.Y - enemyPos.Z);
        ic::vector2df vecPlayer = ic::vector2df(playerPosition.X - enemyPos.X, playerPosition.Z - enemyPos.Z);

        if(vecGauche.dotProduct(vecPlayer) > 0 && vecDroite.dotProduct(vecPlayer) > 0)//Permet de s'assurer que le joueur est devant l'ennemi et non derriere
        {
            float determinantGauche = vecGauche.X * vecPlayer.Y - vecGauche.Y * vecPlayer.X;
            float determinantDroit = vecDroite.X * vecPlayer.Y - vecDroite.Y * vecPlayer.X;

            bool signeGauche = determinantGauche>0;
            bool signeDroit = determinantDroit>0;

            if((signeGauche && !signeDroit) || (!signeGauche && signeDroit))//Verification que les signes sont opposés => donc le joueur est dans le cone
            {
                //Raycast de l'ennemi vers le joueur et verification de hit
                ic::line3d<irr::f32> ray;
                ray.start = enemyPos;
                ray.end = ray.start + (playerPosition - ray.start).normalize() * 1000.0f;


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
                {
                   if(selectedSceneNode->getID() == ID_PLAYER)
                       return true;
                }
            }
        }
    }
    return false;
}

const int& Enemy::getAngleViewEnemy() const
{
    return angleViewEnemy;
}
