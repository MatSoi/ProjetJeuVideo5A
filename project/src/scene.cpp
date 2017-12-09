#include "scene.h"

Scene::Scene()
{
    // Création de la fenêtre et du système de rendu.
    device = createDevice(iv::EDT_OPENGL,
                          ic::dimension2d<u32>(1280, 960),
                          16, false, false, false, &receiver);
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    gui = device->getGUIEnvironment();
    collMan = smgr->getSceneCollisionManager();
    cursor = device->getCursorControl();

    // Création de la GUI
    // Choix de la police de caractères
    skin = gui->getSkin();
    font = gui->getFont("data/fontlucida.png");
    skin->setFont(font);

    // La barre de menu
    menu = new Menu();
    menu->create_menu(gui);
    // Une fenêtre pour différents réglages
    menu->create_window(gui);
}

void Scene::init()
{
    initMap();
    initTextures();
    initPlayer();
    initEnemy();
    initCamera();
    initArrowDebug();
    initReceiver();
}

void Scene::initTextures()
{
    textures.push_back(driver->getTexture("data/base.pcx"));
    textures.push_back(driver->getTexture("data/red_texture.pcx"));
    textures.push_back(driver->getTexture("data/blue_texture.pcx"));
}

void Scene::initMap()
{
    //******************** AJOUT DE LA MAP ********************//
    //  Ajout de l’archive  qui  contient  entre  autres  un  niveau  complet
    device ->getFileSystem()-> addFileArchive("data/map-20kdm2.pk3");
    // On  charge  un bsp (un  niveau) en  particulier :
    meshMap = smgr ->getMesh("20kdm2.bsp");
    nodeMap = smgr ->addOctreeSceneNode(meshMap ->getMesh (0), nullptr, 0, 1024);
    //  Translation  pour  que  nos  personnages  soient  dans le décor
    nodeMap ->setPosition(ic:: vector3df ( -1300 , -104 , -1249));
}

void Scene::initPlayer()
{
    // Chargement de notre personnage
    meshPlayer = smgr->getMesh("data/tris.md2");

    // Attachement de notre personnage dans la scène
    nodePlayer = smgr->addAnimatedMeshSceneNode(meshPlayer);
    nodePlayer->setMaterialFlag(iv::EMF_LIGHTING, false);
    nodePlayer->setMaterialTexture(0, textures[2]);
    nodePlayer->setPosition(ic:: vector3df (0, 0, 0));
    nodePlayer->setRotation(ic:: vector3df (0, 0, 0));

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
                                                 ic::vector3df(0, -10, 0),  // gravité
                                                 ic::vector3df(0, -10, 0));  // décalage du centre
    nodePlayer->addAnimator(anim);
    selector->drop();
    anim->drop();

    player = Player(nodePlayer);
}

void Scene::initEnemy()
{
    // Chargement de notre personnage
    meshEnemy = smgr->getMesh("data/tris.md2");

    // Attachement de notre personnage dans la scène
    nodeEnemy = smgr->addAnimatedMeshSceneNode(meshEnemy, 0, 1);
    nodeEnemy->setMaterialFlag(iv::EMF_LIGHTING, false);
    nodeEnemy->setMaterialTexture(0, textures[1]);
    nodeEnemy->setPosition(core:: vector3df ( 100 , 130 , 100));

    // Création du triangle selector
    scene::ITriangleSelector* selector;
    selector = smgr->createOctreeTriangleSelector(nodeMap->getMesh(), nodeMap);
    nodeMap->setTriangleSelector(selector);

    //Calcul radius de la BBox du node Enemy
    const core::aabbox3d<f32>& box = nodeEnemy->getBoundingBox();
    core::vector3df radius = 1.3f*(box.MaxEdge - box.getCenter());
    // Et l'animateur/collisionneur
    scene::ISceneNodeAnimator *anim;
    anim = smgr->createCollisionResponseAnimator(selector,
                                                 nodeEnemy,  // Le noeud que l'on veut gérer
                                                 radius, // "rayons" de la caméra
                                                 ic::vector3df(0, -10, 0),  // gravité
                                                 ic::vector3df(0, -10, 0));  // décalage du centre
    nodeEnemy->addAnimator(anim);
    selector->drop();

    selector = smgr->createTriangleSelector(nodeEnemy);
    nodeEnemy->setTriangleSelector(selector);
    selector->drop();

    anim->drop();

    enemy = Enemy(nodeEnemy);
}

void Scene::initCamera()
{
    // Placement camera
    camera = smgr->addCameraSceneNode();
}

void Scene::initReceiver()
{
    receiver.set_collision_manager(collMan);
    receiver.set_gui(gui);
    receiver.set_player(&player);
    receiver.set_camera(camera, cursor, device->getVideoDriver()->getScreenSize().Width, device->getVideoDriver()->getScreenSize().Height);
    receiver.set_textures(textures);
    receiver.set_debug(arrowParentDebug);
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

void Scene::playerAttack()
{
    std::vector<int> attack = player.attack(collMan, camera);
    int ID = attack[0];
    int dist = attack[1];

    if (ID != -1)
    {
        std::wstring wstr = L"ID: " + std::to_wstring(ID) + L" Distance: "+ std::to_wstring(dist);
        debugDisplay(wstr, 1);
        if (ID == 1 && dist <= ATTACK_DIST)
            enemy.getHitted();
    }
}

void Scene::debugDisplay(std::wstring wstr, int ind)
{
    const wchar_t* szName = wstr.c_str();
    menu->window->getElementFromId(ind)->setText(szName);
}

void Scene::run()
{
    float width, height;

    u32 then = device->getTimer()->getTime();
    while(device->run())
    {
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;

        width = device->getVideoDriver()->getScreenSize().Width;
        height = device->getVideoDriver()->getScreenSize().Height;

        if(receiver.event_handler(frameDeltaTime, width, height))
            playerAttack();

        driver->beginScene(true, true, iv::SColor(0,50,100,255));

        // Dessin de la scène :
        smgr->drawAll();
        // Dessin de l'interface utilisateur :
        gui->drawAll();

        driver->endScene();
    }
    device->drop();
}
