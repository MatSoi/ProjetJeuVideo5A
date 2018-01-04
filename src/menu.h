#ifndef MENU_H
#define MENU_H

/*!
 * \file menu.h
 * \brief Contient la classe Menu pour creer le menu et les fenetres d iteractions
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include <irrlicht.h>

namespace ic = irr::core;
namespace ig = irr::gui;

/**
 * @brief Classe Menu
 * Gere la creation d un menu et de fenetre d iteractions et de debug
 */
class Menu
{
public:
    /**
     * @brief Creation d un menu en haut de la fenetre de jeu
     * @param gui : pointeur sur le GUI
     */
    void create_menu(ig::IGUIEnvironment *gui);

    /**
     * @brief Creation d une fenetre d affichage de debug
     * @param gui : pointeur sur le GUI
     */
    void create_window(ig::IGUIEnvironment *gui);

    ig::IGUIWindow* window;     /*!< Pointeur sur la fenetre d affichage de debug */
    ig::IGUIContextMenu* menu;  /*!< Pointeur sur menu du jeu */
};

#endif

