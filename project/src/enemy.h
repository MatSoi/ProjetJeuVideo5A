#ifndef ENEMY_H
#define ENEMY_H

/*!
 * \file enemy.h
 * \brief Classe Enemy, qui herite de la classe Characters, et contient les fonctions principales de comportement des ennemis
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include <vector>
#include <irrlicht.h>
#include "characters.h"
#include "ID_list.h"
#include <cmath>
#include <iostream>

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

/**
 * @brief Classe Enemy, herite de la classe Characters
 * Represente les ennemis du joueur, contient les fonctions propres au comportement des ennemis
 */
class Enemy : public Characters
{
public:
    /**
     * @brief Constructeur vide
     */
    Enemy () {}


    /**
     * @brief Constructeur d initialisation.
     * Constructeur appelant simplement celui issu de l heritage avec la classe Characters
     * @param _node : pointeur sur le node de l ennemi
     * @param _animation : premiere animation
     * @param _speed : vitesse de deplacement
     */
    Enemy (is::IAnimatedMeshSceneNode* _node, is::EMD2_ANIMATION_TYPE _animation = is::EMAT_STAND, float _speed = NORMAL_SPEED)
        : Characters (_node, _animation, _speed), angleViewEnemy(20), rayonDetection(300), isAlerted(false) {}

    /**
     * @brief Fonction pour savoir si le joueur est dans le champ de vision de l'ennemi
     * Fonctionnement : -> verification que le joueur se trouve dans le cercle de rayon rayonDetection
     * -> verification que le joueur se trouve dans le cone de l'ennemie :
     *      -> verification qu'il se trouve devant l'ennemie avec un produit scalaire
     *      -> verification qu'il se trouve dans le cone avec un produit vectoriel
     * -> lancer de rayon entre joueur et ennemy pour verifier qu'il y a intersection
     * @param position du joueur
     * @return true si dans le champ de vision de l'enemie, false sinon
     */
    bool playerIsInEnemyView(ic::vector3df playerPosition,is::ISceneCollisionManager* collMan);

    /**
     * @brief Access a l'angle de vue de l'ennemi (1/2 du vrai angle de vision)
     * @return renvoie une reference constante vers l'angle de vision
     */
    const int &getAngleViewEnemy() const;

    /**
     * @brief Fonction appelee si l ennemi est touche par une attaque du joueur
     * Cette fonction n est pas terminee
     * @return true: toujours pour le moment
     */
    bool getHitted();

    /**
    * @brief Surcharge : est appele a la fin de la boucle de l animation de mort de l enemi.
  * Pour le moment : fais disparaitre l ennemi et le deplace en (0, -1000, 0).
    * @param node : pointeur sur le node de l enemi
    */
    void OnAnimationEnd(is::IAnimatedMeshSceneNode *node);

private:

    /**
     * @brief Fonction appelee si l ennemi n a plus de vie
     * @return true: toujours pour le moment
     */
    void die();

    void pain();

    int angleViewEnemy; /*!< Angle de vision de l'ennemi par rapport au centre de sa vision */
    int rayonDetection;
    bool isAlerted;
};

#endif
