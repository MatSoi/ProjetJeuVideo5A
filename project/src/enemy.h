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
#include "path.h"
#include <cmath>
#include <iostream>

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

#define ATTACK_DIST_ENEMY 45

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
    Enemy (is::IAnimatedMeshSceneNode* _node, is::EMD2_ANIMATION_TYPE _animation = is::EMAT_STAND, float _speed = ENEMY_SPEED)
        : Characters (_node, _animation, _speed), angleViewEnemy(30), rayonDetection(300), isPlayerVisible(false), isAlerted(false)
    {
        node->setVisible(true);
        followedPath.initRectangularPath(getPosition(), 100);//Initialisation d'un chemin rectangulaire centre sur l'ennemi
        node->setPosition(followedPath.pathPositions[0]);//La position de l'ennemi est place sur la premiere position du chemin
    }

    /**
     * @brief Fonction pour savoir si le joueur est dans le champ de vision de l'ennemi
     * Fonctionnement : -> verification que le joueur se trouve dans le cercle de rayon rayonDetection
     * -> verification que le joueur se trouve dans le cone de l'ennemie :
     *      -> verification qu'il se trouve devant l'ennemie avec un produit scalaire
     *      -> verification qu'il se trouve dans le cone avec un produit vectoriel
     * -> lancer de rayon entre joueur et ennemy pour verifier qu'il y a intersection
     * @param playerPosition : position du joueur
     * @param collMan : pointeur sur le gestionnaire de collision
     */
    bool isPlayerInEnemyView(const ic::vector3df& playerPosition, irr::scene::ISceneCollisionManager *collMan);

    /**
     * @brief Access a l'angle de vue de l'ennemi (1/2 du vrai angle de vision)
     * @return renvoie une reference constante vers l'angle de vision
     */
    const int &getAngleViewEnemy() const;

    /**
    * @brief Surcharge : est appele a la fin de la boucle de l animation de mort de l enemi.
    * Pour le moment : fais disparaitre l ennemi et le deplace en (0, -1000, 0).
    * Remet a false tous les etats de l ennemi.
    * @param node : pointeur sur le node de l enemi
    */
    void OnAnimationEnd(is::IAnimatedMeshSceneNode *node);

    /**
     * @brief Fonction appelle pour le comportement normal d'un ennemi :
     * -suit le joueur lorsque celui ci est visible
     * -si non visible retour a la position initiale si ce n'est pas deja le cas
     * @param playerPosition : la position du joueur
     * @param frameDeltaTime : delta t entre les frames
     * @param collMan : manager de collision
     */
    bool normalBehaviour(ic::vector3df playerPosition, const irr::f32 frameDeltaTime, irr::scene::ISceneCollisionManager *collMan);

    /**
     * @brief Fonction de prise de degats.
     * L ennemi meurt instantanement s il n est pas en etat d alerte, sinon il peurt un point de vie.
     * Cette fonction appelle ensuite pain() ou die() en fonction des points de vie restant.
     */
    void getHitted();

private:
    /**
     * @brief Set Enemy sur animation de run
     */
    void run();

    /**
     * @brief Set Enemy sur animation de idle sur position d'origine
     */
    void idle();

    /**
     * @brief Methode utilisé pour suivre le joueur, avance simplement la position de l'ennemi en direction du joueur selon le delta T en mettant une animation de run
     * Si la distance entre la position actuelle de l'ennemi et sa position precedemment enregistre est superieur a X, on l'enregistre
     * @param playerPosition : la position actuelle du joueur
     * @param frameDeltaTime : le delta t correspondant a la frame actuelle
     * @param sizePositions : la taille du tableau de positions
     */
    void followPlayer(ic::vector3df playerPosition, const irr::f32 frameDeltaTime);

    /**
     * @brief Fonction de retour a la position initiale de l'ennemi : l'ennemi parcours les positions enregistre jusqu'a arrive a celle de départ
     * @param frameDeltaTime : delta T entre les frames, utilisé pour déplacé l'ennemi
     * @param sizePositions : la taille du tableau de positions
     */
    void getBackToOriginalPosition(const irr::f32 frameDeltaTime);

    /**
     * @brief Fonction de suivi du chemin par l'ennemi, il parcours les positions en s'arretant quelques ms a l'arrivée sur chaque positions
     * @param frameDeltaTime : delta T entre les frames
     */
    void followPath(const irr::f32 frameDeltaTime);

    /**
     * @brief Determine si l ennemi peut attaquer ou non.
     * Il peut attaquer s il n est pas deja en train, s il ne subit pas une attaque (sauf s il lui reste un point de vie unique).
     * @return true si on peut attaquer, false sinon.
     */
    bool canAttack();

    /**
     * @brief Fonction qui passe l ennemi en etat de prise de degat, et qui lance l animation correspondante.
     */
    void pain();

    /**
     * @brief Fonction qui lance l animation de mort.
     */
    void die();

    /**
     * @brief Fonction qui passe l ennemi en etat de prise d attaque, et qui lance l animation correspondante.
     */
    void attack();

    int angleViewEnemy;                 /*!< Angle de vision de l'ennemi par rapport au centre de sa vision */
    int rayonDetection;                 /*!< Rayon de detection de l ennemi */
    bool isPlayerVisible;                 /*!< Booleen indiquant si le joueur est visible par l'ennemi */
    bool isAlerted;                     /*!< Booleen indiquant si l'ennemi est en etat d'alerte */
    int distWithPlayer;                 /*!< distance avec le joueur */
    ic::array<ic::vector3df> alertedPositions; /*!< Positions successives de l'ennemi pour retour en etat non alerte */
    Path followedPath;  /*!< Path suivi par l'ennemi */
};

#endif
