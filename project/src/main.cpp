// main.cpp

#include "scene.h"
#include "scenedebug.h"

int main()
{
    Scene* scene = new Scene();
    scene->init();
    scene->run();

    return 0;
}
