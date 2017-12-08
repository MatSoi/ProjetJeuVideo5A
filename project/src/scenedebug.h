#ifndef SCENEDEBUG_H
#define SCENEDEBUG_H

#include "scene.h"
#include <cmath>

class SceneDebug : public Scene
{
public:
    SceneDebug() : Scene() {}
    void init();
    void run();

private:

    void initBillboardDebug();
    void initSelector();
    void initArrowDebug();
    void enemyRaycastDebug();

    is::ISceneNode * arrowParentDebug;
    std::vector<is::ISceneNode *> arrowsDebug;

    std::vector<scene::IBillboardSceneNode *> bill;
    scene::ISceneNode* highlightedSceneNode;
    video::SMaterial material;
    int angleConeEnemy=20;
};

#endif // SCENEDEBUG_H
