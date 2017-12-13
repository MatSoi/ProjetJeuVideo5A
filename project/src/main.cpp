/*!
 * \file main.cpp
 * \brief Lancement de la scene ou de la version debug
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include "scene.h"
#include "scenedebug.h"

int main()
{
    SceneDebug* scene = new SceneDebug();
    scene->init();
    scene->run();

    return 0;
}
