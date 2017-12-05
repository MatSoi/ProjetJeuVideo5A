// scene.h

#ifndef SCENE_H
#define SCENE_H

#include <irrlicht.h>
#include <iostream>

#include "menu.h"
#include "events.h"
#include "gui_ids.h"
#include "characters.h"
#include "player.h"
#include "enemy.h"

namespace is = irr::scene;
namespace iv = irr::video;

class Scene
{
public:
    Scene ();

    /// fonction d initilisation global de la scene
    void init();

    /// fonction d actualisation de la scene
    void run();

private:
    /// fonction d initialisation de la Map
    void initSceneMap();

    /// fonction d initialisation du joueur
    void initScenePlayer();

    /// initialisation du receveur d evenements
    void initSceneReceiver();

    /// Initialisation des fleches de debug (axes x,y,z)
    void initArrowDebug();

    /// initialisation des enemy
    void initSceneEnemy();


    /// Attributs de scene
    EventReceiver receiver;                 // contient les gestionnaires d evenements
    IrrlichtDevice *device;
    iv::IVideoDriver    *driver;
    is::ISceneManager   *smgr;
    ig::IGUIEnvironment *gui;
    ig::IGUISkin* skin;
    ig::IGUIFont* font;
    ig::ICursorControl* cursor;
    is::ICameraSceneNode *camera;
    Menu* menu;

    /// Attribut de Map
    is:: IMeshSceneNode *nodeMap;

    /// Attribut du joueur
    Player player;
    is::IAnimatedMeshSceneNode *nodePlayer;
    std::vector<is::IAnimatedMeshSceneNode *> nodeEnemy;
    std::vector<Enemy> enemy;
    std::vector<iv::ITexture*> textures;    // contient les differentes textures du joueur

    ///
    is::ISceneNode * arrowParentDebug;
    std::vector<is::ISceneNode *> arrowsDebug;
};



#endif
