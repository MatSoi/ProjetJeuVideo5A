// main.cpp

#include <irrlicht.h>
#include <iostream>

#include "menu.h"
#include "events.h"
#include "gui_ids.h"
#include "characters.h"
#include "player.h"

namespace is = irr::scene;
namespace iv = irr::video;

int main()
{
    // Le gestionnaire d'événements
    EventReceiver receiver;
    std::vector<iv::ITexture*> textures;

    // Création de la fenêtre et du système de rendu.
    IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                          ic::dimension2d<u32>(640, 480),
                                          16, false, false, false, &receiver);
    iv::IVideoDriver    *driver = device->getVideoDriver();
    is::ISceneManager   *smgr = device->getSceneManager();
    ig::IGUIEnvironment *gui = device->getGUIEnvironment();


    //******************** AJOUT DE LA MAP ********************//
    //  Ajout de l’archive  qui  contient  entre  autres  un  niveau  complet
    device ->getFileSystem()-> addFileArchive("data/map-20kdm2.pk3");
    // On  charge  un bsp (un  niveau) en  particulier :
    is:: IAnimatedMesh *meshMap = smgr ->getMesh("20kdm2.bsp");
    is:: IMeshSceneNode *nodeMap;
    nodeMap = smgr ->addOctreeSceneNode(meshMap ->getMesh (0), nullptr , -1, 1024);
    //  Translation  pour  que  nos  personnages  soient  dans le décor
    nodeMap ->setPosition(core:: vector3df ( -1300 , -104 , -1249));

    ///////// Debug ///////////
    is::IAnimatedMeshSceneNode* nodeMapDebug = smgr->addAnimatedMeshSceneNode(meshMap);
    nodeMapDebug->setPosition(core:: vector3df ( -1300 , -103 , -1249));
    u32 debug_info = nodeMapDebug->isDebugDataVisible();
    nodeMapDebug->setDebugDataVisible(debug_info ^ is::EDS_MESH_WIRE_OVERLAY);
    ///////////////////////////

    // Chargement de notre personnage
    is::IAnimatedMesh *mesh = smgr->getMesh("data/tris.md2");

    // Attachement de notre personnage dans la scène
    is::IAnimatedMeshSceneNode *nodePlayer = smgr->addAnimatedMeshSceneNode(mesh);
    nodePlayer->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    //nodePlayer->setMD2Animation(irr::scene::EMAT_STAND);
    textures.push_back(driver->getTexture("data/base.pcx"));
    textures.push_back(driver->getTexture("data/red_texture.pcx"));
    textures.push_back(driver->getTexture("data/blue_texture.pcx"));
    nodePlayer->setMaterialTexture(0, textures[0]);
    nodePlayer->setPosition(core:: vector3df ( 100 , 130 , 100));

    // Placement camera
    is::ICameraSceneNode *camera;
    camera = smgr->addCameraSceneNode();
    //    camera = smgr ->addCameraSceneNodeFPS ();
    //    camera = smgr->addCameraSceneNodeMaya();

    // Création du triangle selector
    scene::ITriangleSelector* selector;
    selector = smgr->createOctreeTriangleSelector(nodeMap->getMesh(), nodeMap);
    nodeMap->setTriangleSelector(selector);

    //Calcul radius de la BBox du node player
    const core::aabbox3d<f32>& box = nodePlayer->getBoundingBox();
    core::vector3df radius = 1.3f*(box.MaxEdge - box.getCenter());
    // Et l'animateur/collisionneur
    scene::ISceneNodeAnimator *anim;
    anim = smgr->createCollisionResponseAnimator(selector,
                                                 nodePlayer,  // Le noeud que l'on veut gérer
                                                 radius, // "rayons" de la caméra
                                                 ic::vector3df(0, -0.5, 0),  // gravité
                                                 ic::vector3df(0, -10, 0));  // décalage du centre
    nodePlayer->addAnimator(anim);
    selector->drop();
    anim->drop();

    /// TEST ///
    Player player(nodePlayer);

    receiver.set_gui(gui);
    receiver.set_player(&player);
    ig::ICursorControl* cursor = device->getCursorControl();
    receiver.set_camera(camera, cursor, device->getVideoDriver()->getScreenSize().Width, device->getVideoDriver()->getScreenSize().Height);
    receiver.set_textures(textures);

//    device->getCursorControl()->setVisible(false);

    // Création de la GUI
    // Choix de la police de caractères
    ig::IGUISkin* skin = gui->getSkin();
    ig::IGUIFont* font = gui->getFont("data/fontlucida.png");
    skin->setFont(font);

    // La barre de menu
    create_menu(gui);
    // Une fenêtre pour différents réglages
    create_window(gui);

    u32 then = device->getTimer()->getTime();

    while(device->run())
    {
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;
        receiver.event_handler(frameDeltaTime, device->getVideoDriver()->getScreenSize().Width, device->getVideoDriver()->getScreenSize().Height);

        driver->beginScene(true, true, iv::SColor(0,50,100,255));
//        device->getCursorControl()->setPosition(0.5f,0.5f);

        // Dessin de la scène :
        smgr->drawAll();
        // Dessin de l'interface utilisateur :
        gui->drawAll();

        driver->endScene();
    }
    device->drop();

    return 0;
}
