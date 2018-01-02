#ifndef PLAYER_H
#define PLAYER_H

/*!
 * \file player.h
 * \brief Classe Player qui herite de la classe Characters et comporte les fonctions principales du joueur
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include <vector>
#include <irrlicht.h>
#include <iostream>

#include "characters.h"
#include "ID_list.h"

#define END_MAP_Y -2000.0f

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

/**
 * @brief Classe Player, herite de la classe Characters
 * Represente le joueur, contient les fonctions propres à ses mouvements/positions
 */
class Player : public Characters
{  
public:
    /**
     * @brief Constructeur vide
     */
    Player () {}

    /**
     * @brief Constructeur du joueur
     * Appelle du constructeur de la classe characters
     * @param _node : pointeur sur le node du joueur
     * @param _animation : premiere animation
     * @param _speed : vitesse de deplacement
     */
    Player (is::IAnimatedMeshSceneNode* _node, is::EMD2_ANIMATION_TYPE _animation = is::EMAT_STAND, float _speed = NORMAL_SPEED)
        : Characters (_node, _animation, _speed), isFurtive(false), isWalking(false) {}

    /**
     * @brief Destructeur
     */
    virtual ~Player () {}

    /**
     * @brief Fonction de deplacement du node player en fonction de l'angle donnee
     * @param frameDeltaTime : temps écoulé entre la derniere frame et la frame actuelle
     * @param angle : angle utilise pour le deplacement
     */
    void move (const f32 frameDeltaTime, float angle);

    /**
     * @brief Fonction de saut du node player
     * @param frameDeltaTime : temps écoulé entre la derniere frame et la frame actuelle
     */
    void jump (const f32 frameDeltaTime);

    /**
     * @brief Fonction de switch entre mode "stealth" et "normal" du joueur
     */
    void setStealth ();

    /**
     * @brief Fonction qui fixe l'animation du joueur a l'animation "idle"
     */
    void setIdle ();

    /**
     * @brief Fonction qui fixe l'animation du joueur a l'animation "walk"
     */
    void setWalkAnimation ();

    /**
     * @brief Fonction d attaque du joueur.
     * Effectue un lancer de rayon de la tête du joueur dans la direction de la camera pour essayer de toucher
     * le/les ennemis.
     * @param collMan : Manager de collision de la scene
     * @param camera : camera de la scene
     * @param angleCamera : angle de la camera, pour faire tourner le joueur dans la bonne orientation
     * @return retour : vector contenant l ID de l ennemi touche et la distance
     * (si on ne touche rien, ce vector contient juste {-1, -1}
     */
    std::vector<int> attack(scene::ISceneCollisionManager *collMan, const is::ICameraSceneNode* camera, const float &angleCamera);

    /**
     * @brief Fonction appellee lorsque le joueur est touche par un ennemi
     */
    void getHitted();

    /**
     * @brief Fonction d'affichage des informations de debug du node player
     * @param debug_type : designe le type de debug a affiche
     */
    void debug (int debug_type);

    /**
     * @brief Fonction de generation d'un texte d'affichage des coordonnes du joueur
     * @return : texte wstring pour affichage avec irrlicht
     */
    std::wstring to_string() const;

    /**
     * @brief Surcharge : est appele a la fin de la loupe de l animation d attaque du joueur
     * Pour le moment : retour a la normale en reappliquant l animation courante.
     * @param node : pointeur sur le node du joueur
     */
    void OnAnimationEnd(is::IAnimatedMeshSceneNode* node);

    /**
     * @brief Renseigne de l etat vivant ou mort du joueur.
     * @return true si le joueur est mort, false sinon.
     */
    bool isDead();

private:
    /**
     * @brief Fonction qui passe l ennemi en etat de prise de degat, et qui lance l animation correspondante.
     */
    void pain();

    /**
     * @brief Fonction qui lance l animation de mort du joueur.
     */
    void die();

    /**
     * @brief Tourne le joueur dans le sens de l angle en parametre.
     * @param angleCamera : reference constante sur l angle de la camera.
     */
    void setLook(const float &angleCamera);

    /**
     * @brief Lance l animation d attaque.
     */
    void animAttack();

    bool isFurtive;     /*!< bool designant si le joueur est en mode furtif */
    bool isWalking;     /*!< bool designant si le joueur est en train de marcher */
};

/**
 * @brief Adaptation de l'operateur de flux pour affichage des informations de debug du player
 * @param o : flux d'entree
 * @param player : pointeur sur le player
 * @return : flux de sortie
 */
inline std::ostream& operator<<(std::ostream& o, Player *player)
{
    ic::vector3df p = player->getPosition();
    o<<"Joueur: x["<<p.X<<"], y["<<p.Y<<"], z[" << p.Z << "]" << std::endl;
    return o;
}

#endif
