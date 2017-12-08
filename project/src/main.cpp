// main.cpp

#include "scene.h"
#include "scenedebug.h"

int main()
{
    SceneDebug* scene = new SceneDebug();
    scene->init();
    scene->run();

    return 0;
}
