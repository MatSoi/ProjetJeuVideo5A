#ifndef CHARACTERS_H
#define CHARACTERS_H

/*!
 * \file characters.h
 * \brief Contient la classe parente de Player et Enemy, definie les fonctions de base pour tous les personnages
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include <vector>
#include <irrlicht.h>

#define NORMAL_SPEED 150.0f
#define STEALTH_SPEED 100.0f
#define ENEMY_SPEED 80.0f

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

/**
 * @brief Classe Characters
 * Contient les fonctions de base des personnages, ennemis ou joueur.
 */
class Characters : public is::IAnimationEndCallBack
{  
public:
    /**
     * @brief Constructeur vide
     */
    Characters () {}

    /**
     * @brief Constructeur d initilisation
     * @param _node : pointeur sur le node du personnage
     * @param _animation : premiere animation du personnage
     * @param _speed : vitesse par defaut du personnage
     */
    Characters(is::IAnimatedMeshSceneNode* _node, is::EMD2_ANIMATION_TYPE _animation, float _speed)
        : node(_node), animation(_animation), speed(_speed) {node->setMD2Animation(animation);}

    /**
     * @brief Destructeur
     */
    virtual ~Characters() {}

    /**
     * @brief Mise a jour de l animation
     * Remplace l animation du personnage par celle en parametre si celle-ci est differente.
     * @param _animation : type d animation
     */
    void updateAnimation (is::EMD2_ANIMATION_TYPE _animation);

    /**
     * @brief Renvoie la position du personnage
     * @return vec3df : reference constante de la position du personnage
     */
    const ic::vector3df &getPosition() const;

    /**
     * @brief Renvoie l orientation du personnage
     * @return vec3df : reference constante de l orientation du personnage
     */
    const ic::vector3df &getRotation() const;

    /**
     * @brief Fonction virtuelle de prise de degat
     * @return aucun dans cette classe
     */
    virtual bool getHitted() = 0;

protected:
    is::IAnimatedMeshSceneNode* node;   /*!< pointeur sur le node du personnage */
    is::EMD2_ANIMATION_TYPE animation;  /*!< type d animation actuelle affectee au personnage */
    float speed;                        /*!< vitesse de deplacement du personnage */
};

#endif
