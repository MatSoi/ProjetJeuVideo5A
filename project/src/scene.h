#ifndef SCENE_H
#define SCENE_H

#include "player.h"
#include "enemy.h"
#include "events.h"
#include "menu.h"
#include "ID_list.h"
#include "game_states.h"
#include "screenManager.h"
#include <map>
#include <ctime>
#include <cstdlib>

/*!
 * \file scene.h
 * \brief Classe Scene qui comporte les differentes initialisations de la scene ainsi que la boucle principale du programme
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#define ATTACK_DIST 50
#define ENEMY_NUMBER 7

/**
 * @brief Classe Scene
 * Represente la scene avec ses attributs et les fonctions d'initialisations ainsi que la boucle principale
 */
class Scene
{
public:
    /**
     * @brief Constructeur de la scene, assure la creation des composantes principales de la scene
     */
    Scene();

    /**
     * @brief Boucle principale du programme
     */
    void run();

protected:
    /**
     * @brief Reinitialise le joueur et les ennemis
     */
    void restartGame();

    /**
     * @brief Fonction d'initialisation globale qui appelle les differentes fonctions d'initialisations elementaires
     */
    void init();

    /**
     * @brief Routine de fonctionement du jeu quand le joueur est en vie.
     * @param frameDeltaTime : temps d actualisation
     * @param playerIsAttacking : true si le joueur attaque, false sinon
     * @param painFrame : frame d affichage de la douleur
     * @param angleCamera : angle horizontal de la camera
     */
    void runTheGame(const f32 frameDeltaTime, const bool &playerIsAttacking, float &painFrame, const float &angleCamera);

    /**
     * @brief Fonction d'initialisation de la map
     */
    void initMap();

    /**
     * @brief Fonction d'initialisation des textures
     */
    void initTextures();

    /**
     * @brief Fonction d'initialisation du joueur
     */
    void initPlayer();

    /**
     * @brief Initialise le collisionneur avec la map pour le node en parametre.
     * @param node : pointeur sur le node.
     * @param radius : reference constante sur le rayon du node associe
     */
    void setupMapCollider(is::IAnimatedMeshSceneNode *node, const ic::vector3df &radius);

    /**
     * @brief Fonction d'initialisation des ennemis
     */
    void initEnemy();

    /**
     * @brief Initialise la map des positions de depart des ennemis
     */
    void initEnemyPosition();

    /**
     * @brief Fonction d'initialisation de la camera
     */
    void initCamera();

    /**
     * @brief Fonction de connection du gestionnaire d'evenements avec les pointeurs des variables necessaires
     */
    void initReceiver();

    /**
     * @brief Fonction appellee lorsque le joueur attaque : tape l'ennemi en face si a distance < ATTACK_DIST
     */
    void playerAttack(const float &angleCamera);

    /**
     * @brief Fonction d'affichage d'element de debug dans la window de debug
     * @param wstr : texte a afficher
     * @param ind : indice de l'element de la window de debug a modifier
     */
    void debugDisplay(std::wstring wstr, int ind);

    // Le gestionnaire d'événements
    EventReceiver receiver;                 /*!< event receiver */
    std::vector<iv::ITexture*> textures;    /*!< tableau des textures des personnages */

    // Création de la fenêtre et du système de rendu.
    IrrlichtDevice *device;                 /*!< pointeur sur le device */
    iv::IVideoDriver    *driver;            /*!< pointeur sur le driver */
    is::ISceneManager   *smgr;              /*!< pointeur sur le manager de scene */
    is::ISceneCollisionManager* collMan;    /*!< pointeur sur le gestionnaire de collision */
    ScreenManager *scManager;               /*!< pointeur sur le manager de l ecran */

    is:: IAnimatedMesh *meshMap;            /*!< pointeur sur le mesh de la map */
    is:: IMeshSceneNode *nodeMap;           /*!< pointeur sur le node de la map */
    scene::ITriangleSelector* selectorMap;  /*!< selecteur de la map */

    Player player;                          /*!< classe joueur */
    is::IAnimatedMesh *meshPlayer;          /*!< pointeur sur le mesh du joueur */
    is::IAnimatedMeshSceneNode *nodePlayer; /*!< pointeur sur le node du joueur */
    core::vector3df radiusPlayer;

    std::map<int, Enemy> enemyMap;                              /*!< map avec cle ID et valeur l'ennemi */
    is::IAnimatedMesh *meshEnemy;                               /*!< pointeur sur le mesh de l'ennemi */
    std::map<int, is::IAnimatedMeshSceneNode *> nodeEnemyMap;   /*!< map avec cle ID et valeur pointeur sur le node de l'ennemi */
    std::map<int, ic::vector3df> positionEnemyMap;              /*!< map avec cle ID et position de depart de l'ennemi */
    core::vector3df radiusEnemy;                                /*!< rayon d un ennemi typique, necessaire pour les collisions apres restart */
    std::map<int, is::IBillboardSceneNode *> billAlerted;       /*!< Map de pointeurs sur les billboards d'affichage d alerte ennemi */

    is::ICameraSceneNode *camera;           /*!< pointeur sur la camera */

    float screen_width, screen_height;      /*!< dimensions de la fenetre de jeu */
    State_List *game_state;                 /*!< Etat du jeu */
    int enemyLeft;                          /*!< nombre d ennemis restants */
};

#endif // SCENE_H
