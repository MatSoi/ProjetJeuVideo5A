// events.cpp

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
EventReceiver::EventReceiver()
    : gui(nullptr), player(nullptr), button_pressed(false), current_texture(0)
{
    //Initialize the key arrays
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
    {
        KeyIsDown[i] = false;
        KeyEvent[i] = false;
    }
}

bool EventReceiver::event_handler(const f32 frameDeltaTime, float width, float height)
{
    screen_width = width;
    screen_height = height;
    mouse_handler(frameDeltaTime);
    keyboard_handler(frameDeltaTime);

    if(arrowParentDebug->isVisible())
        arrowParentDebug->setPosition(player->getPosition());

    std::wstring player_display = player->to_string();
    const wchar_t* szName = player_display.c_str();
    menu->window->getElementFromId(0)->setText(szName);
    return false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard_handler                                        *
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard_handler(const f32 frameDeltaTime)
{
    if(IsKeyDown(KEY_ESCAPE))
        exit(0);

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

    if(IsKeyTriggered(KEY_KEY_I))
        focus_mouse = !focus_mouse;

    //Reset released or first pressed keys
    updateKeyState ();
    return false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::mouse_handler                                           *
\*------------------------------------------------------------------------*/
bool EventReceiver::mouse_handler(const f32 frameDeltaTime)
{
    if (focus_mouse)
    {
        camera_handler();
        cursor->setPosition(0.5f, 0.5f);
    }
    return false;
}

void EventReceiver::camera_handler()
{
    ic::vector3df position;
    ic::vector3df target = player->getPosition() + ic::vector3df(0, 20, 0);
    float theta = MouseState.Position.X * M_PI/180.0f - M_PI_2;
    float phi = MouseState.Position.Y * (-M_PI)/180.0f + M_PI_2;

    position.X = 50.0f * std::sin(phi) * std::sin(theta) + target.X;
    position.Y = 50.0f * std::cos(phi) + target.Y;
    position.Z = 50.0f * std::sin(phi) * std::cos(theta) + target.Z;
    camera->setPosition(position);
    camera->setTarget(target);

    angle_camera = theta + M_PI_2;
}

bool EventReceiver::mouse_event(const SEvent &event)
{
    switch(event.MouseInput.Event)
    {
    case EMIE_LMOUSE_PRESSED_DOWN:
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

    return false;
}

const SMouseState& EventReceiver::GetMouseState(void) const
{
    return MouseState;
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
        return mouse_event(event);
    case EET_GUI_EVENT:
        return gui_handler(event);
    default:;
    }

    return false;
}

/**************************************************************************\
 * EventReceiver::set_node                                                *
\**************************************************************************/
void EventReceiver::set_player(Player* _player)
{
    player = _player;
}

void EventReceiver::set_camera(is::ICameraSceneNode* _camera, ig::ICursorControl* _cursor, float width, float height)
{
    camera =_camera;
    cursor = _cursor;
    screen_width = width;
    screen_height = height;
}

/**************************************************************************\
 * EventReceiver::set_gui                                                 *
\**************************************************************************/
void EventReceiver::set_gui(irr::gui::IGUIEnvironment *g)
{
    gui = g;
}

void EventReceiver::updateKeyState ()
{
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
        KeyEvent[i] = false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::gui_handler                                             *
\*------------------------------------------------------------------------*/
bool EventReceiver::gui_handler(const SEvent &event)
{
    if (!player) return false;
    switch(event.GUIEvent.EventType)
    {
    // Gestion des menus de la barre de menu
    case ig::EGET_MENU_ITEM_SELECTED:
    {
        ig::IGUIContextMenu *menu = (ig::IGUIContextMenu*)event.GUIEvent.Caller;
        s32 item = menu->getSelectedItem();
        s32 id = menu->getItemCommandId(item);

        switch(id)
        {
        case MENU_NEW_GAME:
            // Faire quelque chose ici !
            break;
        case MENU_QUIT:
            exit(0);

        case MENU_BOUNDING_BOX:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            player->debug(is::EDS_BBOX);
            break;

        case MENU_NORMALS:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            player->debug(is::EDS_NORMALS);
            break;

        case MENU_TRIANGLES:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            player->debug(is::EDS_MESH_WIRE_OVERLAY);
            break;

        case MENU_TRANSPARENCY:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            player->debug(is::EDS_HALF_TRANSPARENCY);
            break;

        case MENU_ARROW:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            arrowParentDebug->setVisible(!arrowParentDebug->isVisible());
            break;

        case MENU_ABOUT:
            gui->addMessageBox(L"Boite About", L"Texte présentant ce super jeu\nd'un intérêt incroyable");
            break;
        }
    }
        break;
        // gestion des boites d'édition de texte
    case ig::EGET_EDITBOX_CHANGED:
    {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_VALUE)
        {
            ic::stringc s = event.GUIEvent.Caller->getText();
            //ig::IGUIEditBox *cbox = (ig::IGUIEditBox*)event.GUIEvent.Caller;

            std::cout << "editbox changed:" << s.c_str() << std::endl;
        }
    }
        break;
        // gestion des boutons
    case ig::EGET_BUTTON_CLICKED:
    {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_BUTTON)
            std::cout << "Button clicked\n";
    }
        break;
        // gestion des cases à cocher
    case ig::EGET_CHECKBOX_CHANGED:
    {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_CHECK_BOX)
        {
            std::cout << "Check box clicked: ";
            bool checked = ((ig::IGUICheckBox*)event.GUIEvent.Caller)->isChecked();
            if (!checked) std::cout << "un";
            std::cout << "checked\n";
        }
    }
        break;
        // gestion des combo-box
    case ig::EGET_COMBO_BOX_CHANGED:
    {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_COMBO_BOX)
        {
            ig::IGUIComboBox *cbox = (ig::IGUIComboBox*)event.GUIEvent.Caller;
            s32 item = cbox->getSelected();
            u32 elem_id = cbox->getItemData(item);
            std::cout << "Combo box changed: item " << item << ", id " << elem_id << std::endl;
        }
    }
        break;
        // Gestion des listes
    case ig::EGET_LISTBOX_CHANGED:
    {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_LIST_BOX)
        {
            ig::IGUIListBox *lbox = (ig::IGUIListBox*)event.GUIEvent.Caller;
            s32 item = lbox->getSelected();
            std::cout << "List box changed: item " << item << std::endl;
        }
    }
        break;
        // Gestion des barres de défilement
    case ig::EGET_SCROLL_BAR_CHANGED:
    {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_SCROLLBAR)
        {
            ig::IGUIScrollBar *scroll = (ig::IGUIScrollBar*)event.GUIEvent.Caller;
            s32 value = scroll->getPos();
            std::cout << "Scrollbar moved: " << value << std::endl;
        }
    }
        break;
        // Gestion des spinbox
    case ig::EGET_SPINBOX_CHANGED:
    {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_SPIN_BOX)
        {
            ig::IGUISpinBox *spin = (ig::IGUISpinBox*)event.GUIEvent.Caller;
            f32 value = spin->getValue();
            std::cout << "Spin Box changed: " << value << std::endl;
        }
    }
        break;
    default:;
    }
    return false;
}

