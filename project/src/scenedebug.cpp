#include "scenedebug.h"

void SceneDebug::initBillboardDebug()
{
    // Add as much billboard for the angle.
    for (int i=0; i<2*enemy.getAngleViewEnemy();i++)
        bill.push_back(smgr->addBillboardSceneNode());

    for (int i=0; i<2*enemy.getAngleViewEnemy();i++)
    {
        bill[i]->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
        bill[i]->setMaterialTexture(0, driver->getTexture("data/particle.bmp"));
        bill[i]->setMaterialFlag(video::EMF_LIGHTING, false);
        bill[i]->setMaterialFlag(video::EMF_ZBUFFER, false);
        bill[i]->setSize(core::dimension2d<f32>(20.0f, 20.0f));
        bill[i]->setID(ID_ENEMY); // This ensures that we don't accidentally ray-pick it
    }
}

void SceneDebug::init()
{
    initMap();
    initTextures();
    initPlayer();
    initEnemy();
    initCamera();
    initArrowDebug();
    initReceiver();
    initBillboardDebug();
}

void SceneDebug::enemyRaycastDebug()
{
    for (int i=0; i<2 * enemy.getAngleViewEnemy();i++)
    {
        core::line3d<f32> ray;
        ray.start = nodeEnemy->getPosition();
        ic::vector3df enemyPos= nodeEnemy->getPosition();
        ic::vector3df enemyRot= nodeEnemy->getRotation();
        enemyPos.X += std::cos((enemyRot.Y + float(i - enemy.getAngleViewEnemy())) * M_PI / 180.0f);  // avance selon l angle en parametre
        enemyPos.Z -= std::sin((enemyRot.Y + float(i - enemy.getAngleViewEnemy())) * M_PI / 180.0f);

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
            bill[i]->setVisible(true);
            bill[i]->setPosition(intersection);
        }
        else
            bill[i]->setVisible(false);
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
    float width, height;

    u32 then = device->getTimer()->getTime();

    while(device->run())
    {
        //ROTATION
        nodeEnemy->setRotation(ic::vector3df(0,35,0));

        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;

        width = device->getVideoDriver()->getScreenSize().Width;
        height = device->getVideoDriver()->getScreenSize().Height;

        if(receiver.event_handler(frameDeltaTime, width, height))
            playerAttack();

        if(enemy.behavior(player.getPosition(), collMan)) {
            player.getHitted();
        }

        driver->beginScene(true, true, iv::SColor(0,50,100,255));
        enemyRaycastDebug();
        arrowParentDebug->setPosition(player.getPosition());

        // Dessin de la scène :
        smgr->drawAll();
        // Dessin de l'interface utilisateur :
        gui->drawAll();

        driver->endScene();
    }
    device->drop();
}
