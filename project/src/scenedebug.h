#ifndef SCENEDEBUG_H
#define SCENEDEBUG_H

/*!
 * \file scenedebug.h
 * \brief Classe SceneDebug qui herite de la classe Scene
 * Elle est utilise seulement pour le debug, permettant d'avoir des options de debug constamment affiche
 * \author SOIGNON Matthieu et PASTOR Mickael
 */


#include "scene.h"
#include <cmath>

/**
 * @brief Classe SceneDebug
 * Permet l'affichage constant de certaines options de debug supplementaires
 */
class SceneDebug : public Scene
{
public:
    /**
     * @brief Constructeur de SceneDebug qui appelle le constructeur de Scene
     */
    SceneDebug() : Scene() {}

    /**
     * @brief Surchage de la fonction d'initialisation de scene pour initialiser aussi les composants de debug
     */
    void init();

    /**
     * @brief Surchage de la boucle principale pour afficher option de debug
     */
    void run();

private:

    /**
     * @brief Fonction d'initialisation des billboard utilise pour afficher la vision des ennemis
     */
    void initBillboardDebug();

    /**
     * @brief Fonction d'initialisation des selecteurs sur les personnages
     */
    void initSelector();

    /**
     * @brief Fonction d'initialisation des fleches d'affichages des axes x,y et z
     */
    void initArrowDebug();

    /**
     * @brief Fonction de lancer de rayons a partir de l'ennemi afin d'afficher sa vision
     */
    void enemyRaycastDebug();

    is::ISceneNode * arrowParentDebug;/*!< Pointeur sur le repere d'affichage des axes x,y et z */
    std::vector<is::ISceneNode *> arrowsDebug;/*!< Tableau des pointeurs sur les axes x,y et z */
    is::ISceneCollisionManager* collManDebug;/*!< Pointeur sur le manager de collision */

    std::vector<scene::IBillboardSceneNode *> bill;/*!< Tableau de pointeurs sur les billboards d'affichage de la vision ennemi */
    int angleConeEnemy=20;/*!< Angle de vision de l'ennemi par rapport au centre de sa vision */
};

#endif // SCENEDEBUG_H
