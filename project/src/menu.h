// menu.h

#ifndef MENU_H
#define MENU_H


#include <irrlicht.h>

namespace ic = irr::core;
namespace ig = irr::gui;

class Menu
{
public:
    void create_menu(ig::IGUIEnvironment *gui);
    void create_window(ig::IGUIEnvironment *gui);
    bool isDebugWindowVisible();

    ig::IGUIWindow* window ;
};

#endif

