#include "scene.h"

Scene::Scene ()
{
    // Création de la fenêtre et du système de rendu.
    device = createDevice(iv::EDT_OPENGL,
                          ic::dimension2d<u32>(1280, 960),
                          16, false, false, false, &receiver);
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    gui = device->getGUIEnvironment();
    cursor = device->getCursorControl();
    camera = smgr->addCameraSceneNode();

    skin = gui->getSkin();
    font = gui->getFont("data/fontlucida.png");
    skin->setFont(font);

    textures.push_back(driver->getTexture("data/base.pcx"));
    textures.push_back(driver->getTexture("data/red_texture.pcx"));
    textures.push_back(driver->getTexture("data/blue_texture.pcx"));

    // La barre de menu
    menu->create_menu(gui);
    // Une fenêtre pour différents réglages
    menu->create_window(gui);
}

void Scene::init()
{
    initSceneMap();
    initScenePlayer();
    initSceneEnemy();
    initArrowDebug();
    initSceneReceiver();
}

void Scene::initSceneMap()
{
    //******************** AJOUT DE LA MAP ********************//
    //  Ajout de l’archive  qui  contient  entre  autres  un  niveau  complet
    device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");
    // On  charge  un bsp (un  niveau) en  particulier :
    is:: IAnimatedMesh *meshMap = smgr ->getMesh("20kdm2.bsp");
    nodeMap = smgr ->addOctreeSceneNode(meshMap ->getMesh (0), nullptr , -1, 1024);
    //  Translation  pour  que  nos  personnages  soient  dans le décor
    nodeMap ->setPosition(ic::vector3df ( -1300 , -104 , -1249));

    meshMap->drop();
}

void Scene::initScenePlayer()
{
    // Chargement de notre personnage
    is::IAnimatedMesh *mesh = smgr->getMesh("data/tris.md2");
    // Attachement de notre personnage dans la scène
    nodePlayer = smgr->addAnimatedMeshSceneNode(mesh);
    nodePlayer->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    nodePlayer->setMaterialTexture(0, textures[2]);
    nodePlayer->setPosition(core:: vector3df (100 , 130 , 100));

    // Création du triangle selector
    is::ITriangleSelector* selector;
    selector = smgr->createOctreeTriangleSelector(nodeMap->getMesh(), nodeMap);
    nodeMap->setTriangleSelector(selector);

    //Calcul radius de la BBox du node player
    const ic::aabbox3d<f32>& box = nodePlayer->getBoundingBox();
    ic::vector3df radius = 1.3f*(box.MaxEdge - box.getCenter());
    // Et l'animateur/collisionneur
    scene::ISceneNodeAnimator *anim;
    anim = smgr->createCollisionResponseAnimator(selector,
                                                 nodePlayer,  // Le noeud que l'on veut gérer
                                                 radius, // "rayons" de la caméra
                                                 ic::vector3df(0, -10, 0),  // gravité
                                                 ic::vector3df(0, -10, 0));  // décalage du centre
    nodePlayer->addAnimator(anim);
    selector->drop();
    anim->drop();
    mesh->drop();

    player = Player(nodePlayer);
}

void Scene::initSceneEnemy()
{
    // Chargement de l'enemie
    is::IAnimatedMesh *mesh = smgr->getMesh("data/tris.md2");
    // Attachement de notre personnage dans la scène
    nodeEnemy.push_back(smgr->addAnimatedMeshSceneNode(mesh));
    nodeEnemy[0]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    nodeEnemy[0]->setMaterialTexture(0, textures[1]);
    nodeEnemy[0]->setPosition(core:: vector3df (120, 130, 100));

    // Création du triangle selector
    is::ITriangleSelector* selector;
    selector = smgr->createOctreeTriangleSelector(nodeMap->getMesh(), nodeMap);
    nodeMap->setTriangleSelector(selector);

    //Calcul radius de la BBox du node player
    const ic::aabbox3d<f32>& box = nodeEnemy[0]->getBoundingBox();
    ic::vector3df radius = 1.3f*(box.MaxEdge - box.getCenter());
    // Et l'animateur/collisionneur
    scene::ISceneNodeAnimator *anim;
    anim = smgr->createCollisionResponseAnimator(selector,
                                                 nodeEnemy[0],  // Le noeud que l'on veut gérer
                                                 radius, // "rayons" de la caméra
                                                 ic::vector3df(0, -10, 0),  // gravité
                                                 ic::vector3df(0, -10, 0));  // décalage du centre
    nodeEnemy[0]->addAnimator(anim);
    selector->drop();
    anim->drop();
    mesh->drop();

    enemy.push_back(Enemy(nodeEnemy[0]));
}

void Scene::initSceneReceiver()
{
    receiver.set_debug(arrowParentDebug);
    receiver.set_gui(gui);
    receiver.set_player(&player);
    receiver.set_camera(camera, cursor, device->getVideoDriver()->getScreenSize().Width, device->getVideoDriver()->getScreenSize().Height);
    receiver.set_textures(textures);
    receiver.set_menu(menu);
}

void Scene::initArrowDebug()
{
    //Arrow parent
    arrowParentDebug = smgr->addEmptySceneNode();
    arrowParentDebug->setRotation(ic::vector3df(0,90,0));
    arrowsDebug.resize(3);
    arrowsDebug[0] = smgr->addAnimatedMeshSceneNode(smgr->addArrowMesh("arrowX",
                                                                       iv::SColor(0, 255, 0, 0), iv::SColor(0, 255, 0, 0)), arrowParentDebug);
    arrowsDebug[0]->setRotation(ic::vector3df(90, 0, 0));
    arrowsDebug[0]->setScale(ic::vector3df(5, 10, 5));
    arrowsDebug[0]->setMaterialFlag(video::EMF_LIGHTING, false);

    arrowsDebug[1] = smgr->addAnimatedMeshSceneNode(smgr->addArrowMesh("arrowY",
                                                                       iv::SColor(0, 0, 255, 0), iv::SColor(0, 0, 255, 0)), arrowParentDebug);
    arrowsDebug[1]->setRotation(ic::vector3df(0, 90, 0));
    arrowsDebug[1]->setScale(ic::vector3df(5, 10, 5));
    arrowsDebug[1]->setMaterialFlag(video::EMF_LIGHTING, false);

    arrowsDebug[2] = smgr->addAnimatedMeshSceneNode(smgr->addArrowMesh("arrowZ",
                                                                       iv::SColor(0, 0, 0, 255), iv::SColor(0, 0, 0, 255)), arrowParentDebug);
    arrowsDebug[2]->setRotation(ic::vector3df(0, 0, 90));
    arrowsDebug[2]->setScale(ic::vector3df(5, 10, 5));
    arrowsDebug[2]->setMaterialFlag(video::EMF_LIGHTING, false);
    arrowParentDebug->setVisible(false);
}

void Scene::run()
{
    u32 then = device->getTimer()->getTime();

    while(device->run())
    {
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;
        receiver.event_handler(frameDeltaTime, device->getVideoDriver()->getScreenSize().Width, device->getVideoDriver()->getScreenSize().Height);

        driver->beginScene(true, true, iv::SColor(0,50,100,255));

        // Dessin de la scène :
        smgr->drawAll();
        // Dessin de l'interface utilisateur :
        gui->drawAll();

        driver->endScene();
    }
    device->drop();
}
