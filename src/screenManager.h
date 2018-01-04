#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "menu.h"
#include "gui_ids.h"
#include "irrlicht.h"
#include "game_states.h"
#include <iostream>
#include <vector>

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
     * @param life : valeur de vie du joueur.
     */
    void updateState(float width, float height, int life);

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

    void init_restartGame_button(irr::video::IVideoDriver *driver);

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

    /**
     * @brief Initilialise la barre de vie
     * @param driver : pointeur sur le driver pour charger les images.
     */
    void init_life(irr::video::IVideoDriver *driver);

    /**
     * @brief Met a jour la barre de vie en fonction de la valeur de la vie.
     * @param life : valeur de la vie.
     */
    void updateLife(unsigned int life);

    /**
     * @brief Affiche la barre de vie.
     */
    void displayLife();

    /**
     * @brief Cache la barre de vie.
     */
    void hideLife();

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

    std::vector<ig::IGUIImage*> LifeImage;  /*!< image utilisee pour afficher le game over */
    iv::ITexture *fullHeartTexture;
    iv::ITexture *emptyHeartTexture;

    ig::IGUIButton *newGame_button;         /*!< bouton de nouveau jeu */
    ig::IGUIButton *restartGame_button;     /*!< bouton pour recommencer le jeu */
    ig::IGUIButton *quitGame_button;        /*!< bouton pour quitter le jeu */

    State_List *game_state;                 /*!< Etat du jeu */
};

template<typename T>
/**
 * @brief Affiche une information si elle n est pas deja affichee.
 * @param info : information a afficher
 */
void displayInfo(T info)
{
    if(!info->isVisible())
    {
        info->setVisible(true);
        info->setEnabled(true);
    }
}

template<typename T>
/**
 * @brief Cache une information si elle n est pas deja affichee.
 * @param info : information a cacher
 */
void hideInfo(T info)
{
    if(info->isVisible())
    {
        info->setVisible(false);
        info->setEnabled(false);
    }
}

#endif