#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "menu.h"
#include "irrlicht.h"

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
     * @brief Constructeur d initilisation.
     * @param _gui : pointeur sur l interface utilisateur graphique.
     * @param _cursor : pointeur sur le curseur.
     * @param _painTexture : texture utilisee pour l image des degats.
     * @param _screen_width : largeur de l ecran.
     * @param _screen_height : hauteur de l ecran.
     */
    ScreenManager(ig::IGUIEnvironment *_gui, ig::ICursorControl* _cursor, iv::ITexture *_painTexture, float _screen_width, float _screen_height);

    /**
     * @brief Met a jour les parametres de dimensions de l ecran.
     * Change aussi la taille de l imagede degats en consequence.
     * @param width : largeur de l ecran.
     * @param height : hauteur de l ecran.
     */
    void resize_screen(float width, float height);

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
    ig::IGUIEnvironment *g;                 /*!< pointeur sur l'interface graphique */
    ig::ICursorControl* cursor;             /*!< pointeur sur le curseur de la souris */
    Menu* menu;                             /*!< pointeur sur le menu */

    // Choix de la police de caractères
    ig::IGUISkin* skin;                     /*!< pointeur sur le style de la police d'ecriture */
    ig::IGUIFont* font;                     /*!< pointeur sur la taille de la police d'ecriture */

    ig::IGUIImage* painImage;               /*!< image utilisee pour afficher une prise de degats */
    iv::ITexture *painTexture;              /*!< texture utilisee pour l image des degats */

    float screen_width, screen_height;      /*!< Dimensions de la fenetre de jeu */
};

#endif
