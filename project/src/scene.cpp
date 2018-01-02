/*!
 * \file scene.cpp
 * \brief Implementation des fonctions de la classe Scene.
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include "scene.h"

Scene::Scene() : screen_width(960), screen_height(600), game_state(new State_List(START_SCREEN))
{
    // Création de la fenêtre et du système de rendu.
    device = createDevice(iv::EDT_OPENGL,
                          ic::dimension2d<u32>(screen_width, screen_height),
                          16, false, false, false, &receiver);
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    collMan = smgr->getSceneCollisionManager();
    receiver = EventReceiver(game_state);

    scManager = new ScreenManager(device, driver, screen_width, screen_height, game_state);
}

void Scene::init()
{
    initMap();
    initTextures();
    initPlayer();
    initEnemy();
    initCamera();
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
    nodeMap = smgr ->addOctreeSceneNode(meshMap ->getMesh (0), nullptr, ID_MAP, 1024);
    //  Translation  pour  que  nos  personnages  soient  dans le décor
    nodeMap ->setPosition(ic:: vector3df ( -1300 , -104 , -1249));

    // Création du triangle selector
    selectorMap = smgr->createOctreeTriangleSelector(nodeMap->getMesh(), nodeMap);
    nodeMap->setTriangleSelector(selectorMap);
}

void Scene::initPlayer()
{
    // Chargement de notre personnage
    meshPlayer = smgr->getMesh("data/tris.md2");

    // Attachement de notre personnage dans la scène
    nodePlayer = smgr->addAnimatedMeshSceneNode(meshPlayer, nullptr, ID_PLAYER);
    nodePlayer->setMaterialFlag(iv::EMF_LIGHTING, false);
    nodePlayer->setMaterialTexture(0, textures[2]);
    nodePlayer->setPosition(ic::vector3df (-240.0f , 10.0f, -16.0f));

    //Calcul radius de la BBox du node player
    const core::aabbox3d<f32>& box = nodePlayer->getBoundingBox();
    radiusPlayer = 1.3f*(box.MaxEdge - box.getCenter());
    // Et l'animateur/collisionneur
    scene::ISceneNodeAnimator *anim;
    anim = smgr->createCollisionResponseAnimator(selectorMap,
                                                 nodePlayer,  // Le noeud que l'on veut gérer
                                                 radiusPlayer, // "rayons" de la caméra
                                                 ic::vector3df(0, -10, 0),  // gravité
                                                 ic::vector3df(0, -10, 0));  // décalage du centre
    nodePlayer->addAnimator(anim);
    anim->drop();

    is::ITriangleSelector* selector;
    selector = smgr->createTriangleSelectorFromBoundingBox(nodePlayer);
    nodePlayer->setTriangleSelector(selector);
    selector->drop();

    player = Player(nodePlayer);
}

void Scene::initEnemy()
{
    // Chargement de notre personnage
    meshEnemy = smgr->getMesh("data/tris.md2");

    // Attachement de notre personnage dans la scène
    nodeEnemy = smgr->addAnimatedMeshSceneNode(meshEnemy, 0, ID_ENEMY);
    nodeEnemy->setMaterialFlag(iv::EMF_LIGHTING, false);
    nodeEnemy->setMaterialTexture(0, textures[1]);
    nodeEnemy->setPosition(core:: vector3df (100 , 130, 100));

    //Calcul radius de la BBox du node Enemy
    const core::aabbox3d<f32>& box = nodeEnemy->getBoundingBox();
    core::vector3df radius = 1.3f*(box.MaxEdge - box.getCenter());
    // Et l'animateur/collisionneur
    scene::ISceneNodeAnimator *anim;
    anim = smgr->createCollisionResponseAnimator(selectorMap,
                                                 nodeEnemy,  // Le noeud que l'on veut gérer
                                                 radius, // "rayons" de la caméra
                                                 ic::vector3df(0, -10, 0),  // gravité
                                                 ic::vector3df(0, -10, 0));  // décalage du centre
    nodeEnemy->addAnimator(anim);

    scene::ITriangleSelector* selector;
    selector = smgr->createTriangleSelectorFromBoundingBox(nodeEnemy);
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
    receiver.set_gui(scManager->getGui());
    receiver.set_player(&player);
    receiver.set_camera(camera, scManager->getCursor());
    receiver.set_menu(scManager->getMenu());
}

void Scene::playerAttack(const float &angleCamera)
{
    std::vector<int> attack = player.attack(collMan, camera, angleCamera);
    int ID = attack[0];
    int dist = attack[1];

    if (ID != -1)
    {
        std::wstring wstr = L"ID: " + std::to_wstring(ID) + L" Distance: "+ std::to_wstring(dist);
        debugDisplay(wstr, 1);
        if (ID == ID_ENEMY && dist <= ATTACK_DIST)
            enemy.getHitted();
    }
}

void Scene::debugDisplay(std::wstring wstr, int ind)
{
    const wchar_t* szName = wstr.c_str();
    scManager->getMenu()->window->getElementFromId(ind)->setText(szName);
}

void Scene::run()
{
    init();

    bool playerIsAttacking = false;
    float angleCamera = 0.0f;
    float painFrame = 0;

    u32 then = device->getTimer()->getTime();
    while(device->run())
    {
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;

        screen_width = device->getVideoDriver()->getScreenSize().Width;
        screen_height = device->getVideoDriver()->getScreenSize().Height;


        receiver.event_handler(frameDeltaTime, screen_width, screen_height, playerIsAttacking, angleCamera);

        if(*game_state == RUNNING_GAME)
            runTheGame(frameDeltaTime, playerIsAttacking, painFrame, angleCamera);
        else if(*game_state == RESTART_GAME)
        {
            restartGame();
            *game_state = RUNNING_GAME;
        }

        scManager->updateState(screen_width, screen_height, player.getLife());
        driver->beginScene(true, true, iv::SColor(0,50,100,255));

        // Dessin de la scène :
        smgr->drawAll();
        // Dessin de l'interface utilisateur :
        scManager->getGui()->drawAll();

        driver->endScene();
    }
    device->drop();
}

void Scene::restartGame()
{
    nodePlayer->removeAnimators();
    nodePlayer->setPosition(ic::vector3df(-240.0f , 6.0f, -16.0f));

    scene::ISceneNodeAnimator *anim;
    anim = smgr->createCollisionResponseAnimator(selectorMap,
                                                 nodePlayer,  // Le noeud que l'on veut gérer
                                                 radiusPlayer, // "rayons" de la caméra
                                                 ic::vector3df(0, -10, 0),  // gravité
                                                 ic::vector3df(0, -10, 0));  // décalage du centre
    nodePlayer->addAnimator(anim);

    player = Player(nodePlayer);
    anim->drop();

    enemy = Enemy(nodeEnemy);
}

void Scene::runTheGame(const f32 frameDeltaTime, const bool &playerIsAttacking, float &painFrame, const float &angleCamera)
{
    if(playerIsAttacking)
        playerAttack(angleCamera);

    if(!player.isDead())
    {
        if(enemy.behavior(player.getPosition(), collMan))
        {
            player.getHitted();
            scManager->displayPain(true);
        }
    }
    else
    {
        *game_state = GAME_OVER; // A CHANGER
        scManager->displayPain(false);
    }

    if(scManager->isVisiblePain())
        painFrame += frameDeltaTime;

    if(painFrame > 0.2f)
    {
        painFrame = 0;
        scManager->displayPain(false);
    }
}
