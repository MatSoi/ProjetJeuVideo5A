/*!
 * \file scenedebug.cpp
 * \brief Implementation des fonctions de la classe SceneDebug.
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include "scenedebug.h"

void SceneDebug::initBillboardDebug()
{
    for (std::map<int, ic::vector3df>::iterator it = positionEnemyMap.begin(); it!=positionEnemyMap.end(); ++it)
    {
        // Add as much billboard for the angle.
        for (int i=0; i<2*enemyMap[it->first].getAngleViewEnemy(); ++i)
        {
            bill.push_back(smgr->addBillboardSceneNode());
            bill.back()->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
            bill.back()->setMaterialTexture(0, driver->getTexture("data/particle.bmp"));
            bill.back()->setMaterialFlag(video::EMF_LIGHTING, false);
            bill.back()->setMaterialFlag(video::EMF_ZBUFFER, false);
            bill.back()->setSize(core::dimension2d<f32>(20.0f, 20.0f));
            bill.back()->setID(it->first); // This ensures that we don't accidentally ray-pick it
        }
    }
}

void SceneDebug::init()
{
    initEnemyPosition();
    initMap();
    initTextures();
    initPlayer();
    initEnemy();
    initCamera();
    initReceiver();
}

void SceneDebug::enemyRaycastDebug()
{
    for (std::map<int, ic::vector3df>::iterator it = positionEnemyMap.begin(); it!=positionEnemyMap.end(); ++it)
    {
        for (int i=0; i<2 * enemyMap[it->first].getAngleViewEnemy();i++)
        {
            //Get the index of the billboard
            int index = (std::distance(positionEnemyMap.begin(), it)) * (2 * enemyMap[it->first].getAngleViewEnemy()) + i;

            core::line3d<f32> ray;
            ray.start = nodeEnemyMap[it->first]->getPosition();
            ic::vector3df enemyPos= nodeEnemyMap[it->first]->getPosition();
            ic::vector3df enemyRot= nodeEnemyMap[it->first]->getRotation();
            enemyPos.X += std::cos((enemyRot.Y + float(i - enemyMap[it->first].getAngleViewEnemy())) * M_PI / 180.0f);  // avance selon l angle en parametre
            enemyPos.Z -= std::sin((enemyRot.Y + float(i - enemyMap[it->first].getAngleViewEnemy())) * M_PI / 180.0f);

            ray.end = ray.start + (enemyPos - ray.start).normalize() * 1000.0f;

            // Tracks the current intersection point with the level or a mesh
            core::vector3df intersection;
            // Used to show with triangle has been hit
            core::triangle3df hitTriangle;

            scene::ISceneNode * selectedSceneNode =
                    collMan->getSceneNodeAndCollisionPointFromRay(
                        ray,
                        intersection, // This will be the position of the collision
                        hitTriangle, // This will be the triangle hit in the collision
                        ID_PLAYER | ID_MAP, // This ensures that only nodes that we have
                        // set up to be pickable are considered
                        0); // Check the entire scene (this is actually the implicit default)

            // If the ray hit anything, move the billboard to the collision position
            // and draw the triangle that was hit.
            if(selectedSceneNode)
            {
                bill[index]->setVisible(true);
                bill[index]->setPosition(intersection);
            }
            else
                bill[index]->setVisible(false);
        }
    }
}

void SceneDebug::initArrowDebug()
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
}

void SceneDebug::run()
{
    initArrowDebug();
    initBillboardDebug();

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

        scManager->updateState(screen_width, screen_height, player.getLife(), enemyLeft);

        enemyRaycastDebug();
        arrowParentDebug->setPosition(player.getPosition());


        driver->beginScene(true, true, iv::SColor(0,50,100,255));

        // Dessin de la scène :
        smgr->drawAll();
        // Dessin de l'interface utilisateur :
        scManager->getGui()->drawAll();

        driver->endScene();
    }
    device->drop();
}
