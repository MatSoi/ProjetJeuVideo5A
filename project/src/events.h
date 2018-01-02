#ifndef EVENTS_H
#define EVENTS_H

/*!
 * \file events.h
 * \brief Contient la classe EventReceiver, ainsi que la structure SMouseState
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include <vector>
#include <irrlicht.h>
#include "menu.h"
#include "player.h"
#include "game_states.h"

#define HIGHT_TARGET 30 // position haute de la camera

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

/**
 * @brief Structure SMouseState
 * Structure d etat de la souris
 */
struct SMouseState
{
    /// @brief Constructeur
    SMouseState() : isJustLPressed(false), LeftButtonDown(false) {}

    /// @brief Fonction d update, remet a false le clique souris
    void updateMouse() {isJustLPressed = false;}

    ic::position2di Position;   /*!< position de la souris dans la fenetre de jeu */
    bool isJustLPressed;        /*!< variable du click gauche souris ponctuel, passe a true pendant une frame s il y a un click */
    bool LeftButtonDown;        /*!< variable du click gauche souris constant, reste a true tant que le click gauche est maintenu */
};

/**
 * @brief Classe EventReceiver
 * Cette classe gere les evenements clavier, souris et menu.
 */
class EventReceiver : public IEventReceiver
{
public: 
    /**
     * @brief Constructeur vide
     */
    EventReceiver() {}

    /**
     * @brief Constructeur d initialisation
     * @param _game_state : pointeur sur l etat du jeu
     */
    EventReceiver(State_List *_game_state);

    /**
     * @brief Fonction d evenements surchargee
     * Appelee a chaque input (clavier, souris, menu), elle met a jour les structures,
     * et agit directement sur le jeu en cas d evenments sur le menu
     * @param event : peut correspondre a un evenement clavier, souris, menu
     * @return false : toujours, condition necessaire pour la surcharge
     */
    bool OnEvent(const SEvent &event);

    /**
     * @brief Gestionnaire d evenements
     * Gestionnaire appele a chaque rafraichissement par scene.cpp, appelle les differents gestionnaires d evenements clavier et souris.
     * @param frameDeltaTime : delta de temps entre deux frames, permet des mouvements identiques quelque soit la vitesse de rafraichissement
     * @param width : largeur de la fenetre de jeu
     * @param height : longueur de la fenetre de jeu
     * @param playerIsAttacking : reference passee a true lorsque le joueur attaque
     * @param angleCamera : reference sur l angle horizontal de la camera
     */
    void event_handler(const f32 frameDeltaTime, float width, float height, bool &playerIsAttacking, float &angleCamera);

    /**
     * @brief Initialise le pour sur le GUI
     * @param g : pointeur sur le GUI
     */
    void set_gui(ig::IGUIEnvironment *g) {gui = g;}

    /**
     * @brief Initialise le pointeur sur le joueur
     * @param _player : pointeur sur le joueur
     */
    void set_player(Player* _player) {player = _player;}

    /**
     * @brief Initialise la pointeur sur le camera, le curseur ainsi que les dimensions de la fenetre
     * @param _camera : pointeur sur la camera
     * @param _cursor : pointeur sur le curseur
     */
    void set_camera(is::ICameraSceneNode* _camera, ig::ICursorControl *_cursor) {camera = _camera; cursor = _cursor;}

    /**
     * @brief Initialise de pointeur sur le menu
     * @param _menu : pointeur sur le menu
     */
    void set_menu(Menu *_menu) {menu=_menu;}

private:
    /**
     * @brief Gestionnaire d evenements souris
     * Met a jour la structure MouseState en fcontion de evenements souris
     * @param event : type d evenement souris
     */
    void mouse_event(const SEvent &event);

    /**
     * @brief Gestionnaire d evenements GUI
     * Met a jour la scene en fonction des actions appliqueesau menu
     * @param event :  type d evenements GUI
     */
    void gui_event(const SEvent &event);

    /**
     * @brief Gestionnaire "Action" clavier
     * Affecte des actions et deplacements en fonction des valeurs des structures des evenements clavier
     * @param frameDeltaTime : delta de temps entre deux frames
     */
    void keyboard_handler(const f32 frameDeltaTime);

    /**
     * @brief Gestionnaire "Action" souris
     * Appelle le gestionnaire de camera et repositionne le curseur si les focus est sur la souris
     * @return false s il n y a pas eu de click ou si le focus n est pas sur la souris, true sinon
     */
    bool mouse_handler();

    /**
     * @brief Gestionnaire "Action" camera
     * Bouge la camera en fonction des positions x et y de la souris. La camera se deplace
     * sur une sphere dont le centre correspond environ a la tete de l avatar du joueur
     */
    void camera_handler();

    /**
     * @brief Fait tourner la camera autour de la carte de jeu
     * @param frameDeltaTime : temps d actualisation
     */
    void camera_rotation(const f32 frameDeltaTime);

    /**
     * @brief Mise a jour d une structure d evenements clavier
     * Remet a false toutes les entrees du tableau KeyEvent
     */
    void updateKeyState();

    /**
     * @brief Verifie qu une touche est pressee
     * @param keyCode : touche consideree
     * @return true sila touche est pressee, false sinon
     */
    virtual bool IsKeyDown(EKEY_CODE keyCode) const {return KeyIsDown[keyCode];}

    /**
     * @brief Verifie qu une touche vient d etre relachee
     * @param keyCode : touche consideree
     * @return true si la touche vient d etre relachee, false sinon
     */
    virtual bool IsKeyReleased(EKEY_CODE keyCode) const {return (KeyEvent[keyCode] && !KeyIsDown[keyCode]);}

    /**
     * @brief Verifie qu une touche vient d etre pressee
     * @param keyCode : touche consideree
     * @return true si la touche vient d etre pressee, false sinon
     */
    virtual bool IsKeyTriggered(EKEY_CODE keyCode) const {return (KeyEvent[keyCode] && KeyIsDown[keyCode]);}

    bool KeyIsDown[KEY_KEY_CODES_COUNT];    /*!< Tableau des actions clavier durables */
    bool KeyEvent[KEY_KEY_CODES_COUNT];     /*!< Tableau des actions clavier ephemeres */

    ig::IGUIEnvironment *gui;               /*!< Pointeur sur le GUI */
    Player* player;                         /*!< Pointeur sur le joueur */
    SMouseState MouseState;                 /*!< Structure d etat de la souris */
    is::ICameraSceneNode* camera;           /*!< Pointeur sur la camera */
    gui::ICursorControl *cursor;            /*!< Pointeur sur la curseur */
    Menu* menu;                             /*!< Pointeur sur le menu */

    float angle_camera;                     /*!< Angle horizontal de camera */
    float screen_width, screen_height;      /*!< Dimensions de la fenetre de jeu */
    bool focus_mouse;                       /*!< Booleen a true quand les mouvements souris doivent etre consideres */

    State_List* game_state;                 /*!< Etat du jeu */
};

#endif
