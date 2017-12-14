#ifndef SCENE_H
#define SCENE_H
#include "player.h"
#include "enemy.h"
#include "events.h"
#include "menu.h"
#include "ID_list.h"

/*!
 * \file scene.h
 * \brief Classe Scene qui comporte les differentes initialisations de la scene ainsi que la boucle principale du programme
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#define ATTACK_DIST 50

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
     * @brief Fonction d'initialisation globale qui appelle les differentes fonctions d'initialisations elementaires
     */
    void init();

    /**
     * @brief Boucle principale du programme
     */
    void run();

protected:

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
     * @brief Fonction d'initialisation des ennemis
     */
    void initEnemy();

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
    void playerAttack();

    /**
     * @brief Fonction d'affichage d'element de debug dans la window de debug
     * @param wstr : texte a afficher
     * @param ind : indice de l'element de la window de debug a modifier
     */
    void debugDisplay(std::wstring wstr, int ind);


    // Le gestionnaire d'événements
    EventReceiver receiver;/*!< event receiver */
    std::vector<iv::ITexture*> textures;/*!< tableau des textures des personnages */

    // Création de la fenêtre et du système de rendu.
    IrrlichtDevice *device;/*!< pointeur sur le device */
    iv::IVideoDriver    *driver;/*!< pointeur sur le driver */
    is::ISceneManager   *smgr;/*!< pointeur sur le manager de scene */
    ig::IGUIEnvironment *gui;/*!< pointeur sur l'interface graphique */
    is::ISceneCollisionManager* collMan;/*!< pointeur sur le gestionnaire de collision */
    Menu* menu;/*!< pointeur sur le menu */

    is:: IAnimatedMesh *meshMap;/*!< pointeur sur le mesh de la map */
    is:: IMeshSceneNode *nodeMap;/*!< pointeur sur le node de la map */

    is::IAnimatedMesh *meshPlayer;/*!< pointeur sur le mesh du joueur */
    is::IAnimatedMeshSceneNode *nodePlayer;/*!< pointeur sur le node du joueur */

    is::IAnimatedMesh *meshEnemy;/*!< pointeur sur le mesh de l'ennemi */
    is::IAnimatedMeshSceneNode *nodeEnemy;/*!< pointeur sur le node de l'ennemi */

    is::ICameraSceneNode *camera;/*!< pointeur sur la camera */

    // Choix de la police de caractères
    ig::IGUISkin* skin;/*!< pointeur sur le style de la police d'ecriture */
    ig::IGUIFont* font;/*!< pointeur sur la taille de la police d'ecriture */

    ig::ICursorControl* cursor;/*!< pointeur sur le curseur de la souris */

    Player player;/*!< classe joueur */
    Enemy enemy;/*!< classe enemy */

};

#endif // SCENE_H
