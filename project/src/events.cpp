/*!
 * \file events.cpp
 * \brief Implementation des fonctions de la classe EventReceiver
 * \author SOIGNON Matthieu et PASTOR Mickael
 */


#include <iostream>
#include <irrlicht.h>
#include <cmath>
#include "events.h"
#include "gui_ids.h"

#define PHI_MIN -45.0f
#define PHI_MAX 80.0f


/**************************************************************************\
 * EventReceiver::EventReceiver                                           *
\**************************************************************************/
EventReceiver::EventReceiver(State_List * _game_state)
    : gui(nullptr), player(nullptr), angle_camera(0.0f), focus_mouse(false), game_state(_game_state)
{
    //Initialize the key arrays
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
    {
        KeyIsDown[i] = false;
        KeyEvent[i] = false;
    }
}

void EventReceiver::event_handler(const f32 frameDeltaTime, float width, float height, bool &playerIsAttacking, float &angleCamera)
{
    screen_width = width;
    screen_height = height;
    playerIsAttacking = false;

    if(*game_state != START_SCREEN)
        playerIsAttacking = mouse_handler();
    else
        camera_rotation(frameDeltaTime);

    std::wstring player_display = player->to_string();
    const wchar_t* szName = player_display.c_str();
    menu->window->getElementFromId(0)->setText(szName);

    angleCamera = angle_camera;
    keyboard_handler(frameDeltaTime);
    MouseState.updateMouse();
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard_handler                                        *
\*------------------------------------------------------------------------*/
void EventReceiver::keyboard_handler(const f32 frameDeltaTime)
{
    if(IsKeyDown(KEY_ESCAPE))
        exit(0);
    if(IsKeyTriggered(KEY_KEY_I))
        focus_mouse = !focus_mouse;

    if(*game_state == RUNNING_GAME) {
        if((IsKeyTriggered(KEY_KEY_Z) || IsKeyTriggered(KEY_KEY_S) || IsKeyTriggered(KEY_KEY_D) || IsKeyTriggered(KEY_KEY_Q)))
            player->setWalkAnimation(); // on lance l animation de marche si une des touches de deplacement est pressee
        if((IsKeyReleased(KEY_KEY_Z) || IsKeyReleased(KEY_KEY_S) || IsKeyReleased(KEY_KEY_D) || IsKeyReleased(KEY_KEY_Q)) &&
                !(IsKeyDown(KEY_KEY_Z) || IsKeyDown(KEY_KEY_S) || IsKeyDown(KEY_KEY_D) || IsKeyDown(KEY_KEY_Q)))
            player->setIdle();          // on arrete la marche si une des touches est relachee et que les autres ne sont pas pressees
        // on pourrait enlever la partie IsKeyReleased mais on appelerait tout le temps setIdle

        if(IsKeyTriggered(KEY_KEY_C))
            player->setStealth();

        //KEY DOWN
        if(IsKeyDown(KEY_KEY_Z) && IsKeyDown(KEY_KEY_D))
            player->move(frameDeltaTime, angle_camera + M_PI/4.0f);
        else if(IsKeyDown(KEY_KEY_Z) && IsKeyDown(KEY_KEY_Q))
            player->move(frameDeltaTime, angle_camera - M_PI/4.0f);
        else if(IsKeyDown(KEY_KEY_S) && IsKeyDown(KEY_KEY_D))
            player->move(frameDeltaTime, angle_camera + M_PI - M_PI/4.0f);
        else if(IsKeyDown(KEY_KEY_S) && IsKeyDown(KEY_KEY_Q))
            player->move(frameDeltaTime, angle_camera + M_PI + M_PI/4.0f);
        else if(IsKeyDown(KEY_KEY_Z))
            player->move(frameDeltaTime, angle_camera);
        else if(IsKeyDown(KEY_KEY_S))
            player->move(frameDeltaTime, angle_camera + M_PI);
        else if(IsKeyDown(KEY_KEY_D))
            player->move(frameDeltaTime, angle_camera + M_PI_2);
        else if(IsKeyDown(KEY_KEY_Q))
            player->move(frameDeltaTime, angle_camera - M_PI_2);

        if(IsKeyDown(KEY_SPACE))
            player->jump(frameDeltaTime);
    }

    // Reset released or first pressed keys
    updateKeyState ();
}

/*------------------------------------------------------------------------*\
 * EventReceiver::mouse_handler                                           *
\*------------------------------------------------------------------------*/
bool EventReceiver::mouse_handler()
{
    if (focus_mouse)
    {
        camera_handler();
        cursor->setPosition(0.5f, 0.5f);
        return MouseState.isJustLPressed;
    }

    return false;
}

void EventReceiver::camera_handler()
{
    ic::vector3df position;

    float conv = M_PI/180.0f;
    ic::vector3df target = player->getPosition() + ic::vector3df(0, HIGHT_TARGET, 0);

    float theta = MouseState.Position.X * conv - M_PI_2;
    float phi = MouseState.Position.Y * -conv + M_PI_2;

    position.X = 50.0f * std::sin(phi) * std::sin(theta) + target.X;
    position.Y = 50.0f * std::cos(phi) + target.Y;
    position.Z = 50.0f * std::sin(phi) * std::cos(theta) + target.Z;
    camera->setPosition(position);
    camera->setTarget(target);

    angle_camera = theta + M_PI_2;
}

void EventReceiver::camera_rotation(const f32 frameDeltaTime)
{
    ic::vector3df position;

    float conv = M_PI/180.0f;

    float theta = angle_camera * conv - M_PI_2;
    float phi = 45.0f * -conv + M_PI_2;

    position.X = 1500.0f * std::sin(phi) * std::sin(theta);
    position.Y = 1500.0f * std::cos(phi);
    position.Z = 1500.0f * std::sin(phi) * std::cos(theta);
    camera->setPosition(position);

    angle_camera += 10.0f * frameDeltaTime;
}

void EventReceiver::mouse_event(const SEvent &event)
{
    switch(event.MouseInput.Event)
    {
    case EMIE_LMOUSE_PRESSED_DOWN:
        MouseState.isJustLPressed = !MouseState.LeftButtonDown;
        MouseState.LeftButtonDown = true;
        break;

    case EMIE_LMOUSE_LEFT_UP:
        MouseState.LeftButtonDown = false;
        break;

    case EMIE_MOUSE_MOVED:
        MouseState.Position.X += event.MouseInput.X - int(screen_width/2.0f);
        MouseState.Position.Y += event.MouseInput.Y - int(screen_height/2.0f);
        break;
    default:
        // We won't use the wheel
        break;
    }

    MouseState.Position.Y = std::max(PHI_MIN, std::min(PHI_MAX, float(MouseState.Position.Y)));
}

/**************************************************************************\
 * EventReceiver::OnEvent                                                 *
\**************************************************************************/
bool EventReceiver::OnEvent(const SEvent &event)
{
    if (!player) return false;

    switch (event.EventType)
    {
    case EET_KEY_INPUT_EVENT:
        //Release/FirstPress gestion
        KeyEvent[event.KeyInput.Key] = (KeyIsDown[event.KeyInput.Key] != event.KeyInput.PressedDown);
        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        break;
    case EET_MOUSE_INPUT_EVENT:
        mouse_event(event);
    case EET_GUI_EVENT:
        gui_event(event);
    default:;
    }

    return false;
}

/**************************************************************************\
 * EventReceiver::set_node                                                *
\**************************************************************************/

void EventReceiver::updateKeyState ()
{
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
        KeyEvent[i] = false;
}


/*------------------------------------------------------------------------*\
 * EventReceiver::gui_handler                                             *
\*------------------------------------------------------------------------*/
void EventReceiver::gui_event(const SEvent &event)
{
    if (!player) return;
    switch(event.GUIEvent.EventType)
    {
    // Gestion des menus de la barre de menu
    case ig::EGET_MENU_ITEM_SELECTED:
    {
        ig::IGUIContextMenu *menuSelected = (ig::IGUIContextMenu*)event.GUIEvent.Caller;
        s32 item = menuSelected->getSelectedItem();
        s32 id = menuSelected->getItemCommandId(item);

        switch(id)
        {
        case MENU_NEW_GAME:
            *game_state = RESTART_GAME;
            focus_mouse = true;
            break;
        case MENU_QUIT:
            exit(0);

        case MENU_BOUNDING_BOX:
            menuSelected->setItemChecked(item, !menuSelected->isItemChecked(item));
            player->debug(is::EDS_BBOX);
            break;

        case MENU_NORMALS:
            menuSelected->setItemChecked(item, !menuSelected->isItemChecked(item));
            player->debug(is::EDS_NORMALS);
            break;

        case MENU_TRIANGLES:
            menuSelected->setItemChecked(item, !menuSelected->isItemChecked(item));
            player->debug(is::EDS_MESH_WIRE_OVERLAY);
            break;

        case MENU_TRANSPARENCY:
            menuSelected->setItemChecked(item, !menuSelected->isItemChecked(item));
            player->debug(is::EDS_HALF_TRANSPARENCY);
            break;

        case MENU_DEBUG_BOX:
            menuSelected->setItemChecked(item, !menuSelected->isItemChecked(item));
            menu->window->setVisible(!menu->window->isVisible());
            break;

        case MENU_ABOUT:
            gui->addMessageBox(L"Boite About", L"Texte présentant ce super jeu\nd'un intérêt incroyable");
            break;
        }
    }
        break;
    case ig::EGET_BUTTON_CLICKED:
    {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == NEW_GAME_BUTTON)
        {
            *game_state = RUNNING_GAME;
            focus_mouse = true;
        }
        else if (id == RESTART_GAME_BUTTON)
            *game_state = RESTART_GAME;
        else if (id == QUIT_GAME_BUTTON)
            exit(0);
    }
        break;
    default:;
    }
}
