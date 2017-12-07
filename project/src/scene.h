#ifndef SCENE_H
#define SCENE_H

#include "player.h"
#include "events.h"
#include "menu.h"

class Scene
{
public:
    Scene();
    void init();
    void run();

private:

    void initMap();
    void initPlayer();
    void initCamera();
    void initArrowDebug();
    void initReceiver();


    // Le gestionnaire d'événements
    EventReceiver receiver;
    std::vector<iv::ITexture*> textures;

    // Création de la fenêtre et du système de rendu.
    IrrlichtDevice *device;
    iv::IVideoDriver    *driver;
    is::ISceneManager   *smgr;
    ig::IGUIEnvironment *gui;

    is:: IAnimatedMesh *meshMap;
    is:: IMeshSceneNode *nodeMap;

    is::IAnimatedMesh *meshPlayer;
    is::IAnimatedMeshSceneNode *nodePlayer;

    is::ICameraSceneNode *camera;

    // Choix de la police de caractères
    ig::IGUISkin* skin;
    ig::IGUIFont* font;

    ig::ICursorControl* cursor;

    Player player;

    is::ISceneNode * arrowParentDebug;
    std::vector<is::ISceneNode *> arrowsDebug;
};

#endif // SCENE_H
