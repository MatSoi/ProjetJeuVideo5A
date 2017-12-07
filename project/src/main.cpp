// main.cpp

#include <irrlicht.h>
#include <iostream>

#include "menu.h"
#include "events.h"
#include "gui_ids.h"
#include "characters.h"
#include "player.h"
#include "scene.h"

namespace is = irr::scene;
namespace iv = irr::video;

int main()
{
    Scene scene;
    scene.init();
    scene.run();

    return 0;
}
