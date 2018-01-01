#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "menu.h"
#include "gui_ids.h"
#include "irrlicht.h"
#include "game_states.h"
#include "iostream"

/*!
 * \file screenManager.h
 * \brief Classe ScreenManager qui gere les differents affichages sur l ecran
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

/**
 * @brief Classe ScreenManager
 *  Gere les affichages a faire sur l ecran (gameover, vie, etc...)
 */
class ScreenManager
{
public:
    /**
     * @brief Constructeur d initialisation.
     * @param device : pointeur sur la device
     * @param driver : pointeur sur le driver.
     * @param _screen_width : largeur de l ecran.
     * @param _screen_height : longueur de l ecran.
     * @param _game_state : pointeur sur l etat du jeu.
     */
    ScreenManager(irr::IrrlichtDevice* device, iv::IVideoDriver *driver, float _screen_width, float _screen_height, State_List *_game_state);

    /**
     * @brief Met a jour les parametres de dimensions de l ecran.
     * Change aussi la taille de l imagede degats en consequence.
     * @param width : largeur de l ecran.
     * @param height : hauteur de l ecran.
     */
    void updateState(float width, float height);

    /**
     * @brief Getteur sur le pointeur sur l interface utilisateur graphique.
     * @return g : pointeur sur GUI.
     */
    ig::IGUIEnvironment* getGui() {return g;}

    /**
     * @brief Getteur sur le pointeur du curseur.
     * @return cursor : pointeur sur le curseur.
     */
    ig::ICursorControl* getCursor() {return cursor;}

    /**
     * @brief Getteur sur le pointeur du curseur.
     * @return cursor : pointeur sur le curseur.
     */
    Menu* getMenu() {return menu;}

    /**
     * @brief Affiche ou non l 'image de degats.
     * @param state : si true, alors l image est affichee, sinon elle est mise invisible.
     */
    void displayPain(bool state);

    /**
     * @brief On verifie que l image de degats est visible.
     * @return true si elle est visible, false sinon.
     */
    bool isVisiblePain() const;

private:
    /**
     * @brief Initialise le bouton de nouveau jeu.
     * @param driver : pointeur sur le driver pour charger les images.
     */
    void init_newGame_button(irr::video::IVideoDriver *driver);

    /**
     * @brief Initialise le bouton de quitter le jeu.
     * @param driver : pointeur sur le driver pour charger les images.
     */
    void init_quitGame_button(irr::video::IVideoDriver *driver);

    /**
     * @brief Initialise l image du titre.
     * @param driver : pointeur sur le driver pour charger les images.
     */
    void init_title(irr::video::IVideoDriver *driver);

    /**
     * @brief Initialise l image du game over.
     * @param driver : pointeur sur le driver pour charger les images.
     */
    void init_gameOver(irr::video::IVideoDriver *driver);

    /**
     * @brief Initialise l image de la douleur.
     * @param driver : pointeur sur le driver pour charger les images.
     */
    void init_pain(irr::video::IVideoDriver *driver);

    ig::IGUIEnvironment *g;                 /*!< pointeur sur l'interface graphique */
    ig::ICursorControl* cursor;             /*!< pointeur sur le curseur de la souris */
    Menu* menu;                             /*!< pointeur sur le menu */

    // Choix de la police de caractères
    ig::IGUISkin* skin;                     /*!< pointeur sur le style de la police d'ecriture */
    ig::IGUIFont* font;                     /*!< pointeur sur la taille de la police d'ecriture */

    ig::IGUIImage* painImage;               /*!< image utilisee pour afficher une prise de degats */

    float screen_width, screen_height;      /*!< Dimensions de la fenetre de jeu */

    ig::IGUIImage* titleImage;              /*!< image utilisee pour afficher le titre */
    ig::IGUIImage* gameOverImage;           /*!< image utilisee pour afficher le game over */
    ig::IGUIButton *newGame_button;         /*!< bouton de nouveau jeu */
    ig::IGUIButton *quitGame_button;        /*!< bouton pour quitter le jeu */

    State_List *game_state;                 /*!< Etat du jeu */
};

#endif
