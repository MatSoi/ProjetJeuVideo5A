/*!
 * \file main.cpp
 * \brief Lancement de la scene ou de la version debug
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include "scene.h"
#include "scenedebug.h"

int main()
{
    Scene* scene = new Scene();
    scene->run();

    return 0;
}
