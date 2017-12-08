#ifndef SCENE_H
#define SCENE_H

#include "player.h"
#include "enemy.h"
#include "events.h"
#include "menu.h"

enum
{
    // I use this ISceneNode ID to indicate a scene node that is
    // not pickable by getSceneNodeAndCollisionPointFromRay()
    ID_IsNotPickable = 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be picked by ray selection.
    IDFlag_IsPickable = 1 << 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be highlighted.  In this example, the
    // homonids can be highlighted, but the level mesh can't.
    IDFlag_IsHighlightable = 1 << 1
};

class Scene
{
public:
    Scene();
    void init();
    void run();

protected:

    void initMap();
    void initTextures();
    void initPlayer();
    void initEnemy();
    void initCamera();
    void initReceiver();


    // Le gestionnaire d'événements
    EventReceiver receiver;
    std::vector<iv::ITexture*> textures;

    // Création de la fenêtre et du système de rendu.
    IrrlichtDevice *device;
    iv::IVideoDriver    *driver;
    is::ISceneManager   *smgr;
    ig::IGUIEnvironment *gui;
    is::ISceneCollisionManager* collMan;
    Menu* menu;

    is:: IAnimatedMesh *meshMap;
    is:: IMeshSceneNode *nodeMap;

    is::IAnimatedMesh *meshPlayer;
    is::IAnimatedMeshSceneNode *nodePlayer;

    is::IAnimatedMesh *meshEnemy;
    is::IAnimatedMeshSceneNode *nodeEnemy;

    is::ICameraSceneNode *camera;

    // Choix de la police de caractères
    ig::IGUISkin* skin;
    ig::IGUIFont* font;

    ig::ICursorControl* cursor;

    Player player;
    Enemy enemy;

};

#endif // SCENE_H
