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
        : Characters (_node, _animation, _speed) {}

    /**
     * @brief Fonction appelee si l ennemi est touche par une attaque du joueur.
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
};

#endif
