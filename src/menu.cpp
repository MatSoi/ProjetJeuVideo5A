/*!
 * \file menu.cpp
 * \brief Implementation des fonctions de menu.h
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include <iostream>
#include <irrlicht.h>
#include "menu.h"
#include "gui_ids.h"

using namespace irr;

/*===========================================================================*\
 * create_menu                                                               *
\*===========================================================================*/
void Menu::create_menu(ig::IGUIEnvironment *gui)
{
    ig::IGUIContextMenu *submenu;

    // Les trois entrées principales :
    menu = gui->addMenu();
    menu->addItem(L"File", -1, true, true);
    menu->addItem(L"Debug", -1, true, true);
    menu->addItem(L"Help", -1, true, true);

    // Le contenu du menu File :
    submenu = menu->getSubMenu(0);
    submenu->addItem(L"New game...", MENU_NEW_GAME);
    submenu->addSeparator();
    submenu->addItem(L"Quit", MENU_QUIT);

    // Le contenu du menu Debug :
    submenu = menu->getSubMenu(1);
    submenu->addItem(L"Bounding Box",   MENU_BOUNDING_BOX);
    submenu->addItem(L"Show Normals",   MENU_NORMALS);
    submenu->addItem(L"Show Triangles", MENU_TRIANGLES);
    submenu->addItem(L"Transparency",   MENU_TRANSPARENCY);
    submenu->addItem(L"Debug Box",      MENU_DEBUG_BOX);

    // Le contenu du menu Help :
    submenu = menu->getSubMenu(2);
    submenu->addItem(L"About...", MENU_ABOUT);
}

/*===========================================================================*\
 * create_window                                                             *
\*===========================================================================*/
void Menu::create_window(ig::IGUIEnvironment *gui)
{
    // La fenêtre
    window = gui->addWindow(ic::rect<s32>(0,80, 400,200), false, L"Box de debug");
    window->getCloseButton()->setVisible(false);
    window->setDrawBackground(true);
    window->setVisible(false);

    // Une zone d'édition de texte, précédée d'un label
    gui->addStaticText(L"Position Joueur", ic::rect<s32>(20, 30, 400, 200), false, false, window, 0);
    gui->addStaticText(L"Lancer de rayon", ic::rect<s32>(20, 45, 400, 200), false, false, window, 1);
}
