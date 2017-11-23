// events.cpp

#include <iostream>

#include <irrlicht.h>

#include "events.h"
#include "gui_ids.h"

/**************************************************************************\
 * EventReceiver::EventReceiver                                           *
\**************************************************************************/
EventReceiver::EventReceiver()
    : gui(nullptr), node(nullptr), button_pressed(false), current_texture(0)
{
    //Initialize the key arrays
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
    {
        KeyIsDown[i] = false;
        KeyEvent[i] = false;
    }
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard_handler                                        *
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard_handler(const f32 frameDeltaTime)
{
    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();

    if(IsKeyDown(KEY_ESCAPE))
        exit(0);

    /*if(IsKeyTriggered(KEY_KEY_Z) || IsKeyTriggered(KEY_KEY_S))
        node->setMD2Animation(is::EMAT_RUN);
    if(IsKeyReleased(KEY_KEY_Z) || IsKeyReleased(KEY_KEY_S))
        node->setMD2Animation(is::EMAT_STAND);
    if(IsKeyTriggered(KEY_KEY_D) || IsKeyTriggered(KEY_KEY_Q))
        node->setMD2Animation(is::EMAT_RUN);
    if(IsKeyReleased(KEY_KEY_D) || IsKeyReleased(KEY_KEY_Q))
        node->setMD2Animation(is::EMAT_STAND);*/

    //KEY DOWN
    if(IsKeyDown(KEY_KEY_Z)) {
        position.X += 8 * cos(rotation.Y * M_PI / 180.0);
        position.Z += -8 * sin(rotation.Y * M_PI / 180.0);
    }
    else if(IsKeyDown(KEY_KEY_S)) {
        position.X -= 4 * cos(rotation.Y * M_PI / 180.0);
        position.Z -= -4 * sin(rotation.Y * M_PI / 180.0);
    }
    if(IsKeyDown(KEY_KEY_D))
        rotation.Y += 5;
    else if(IsKeyDown(KEY_KEY_Q))
        rotation.Y -= 5;

    if(IsKeyDown(KEY_SPACE))
        position.Y += 100 * frameDeltaTime;

    node->setPosition(position);
    node->setRotation(rotation);

    //Reset released or first pressed keys
    updateKeyState ();
    return false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::mouse_handler                                           *
\*------------------------------------------------------------------------*/
bool EventReceiver::mouse_handler(const SEvent &event)
{
    /*switch(event.MouseInput.Event)
    {
    case EMIE_LMOUSE_PRESSED_DOWN:
      button_pressed = true;
      old_x = event.MouseInput.X;
      old_y = event.MouseInput.Y;
      break;
    case EMIE_LMOUSE_LEFT_UP:
      button_pressed = false;
      break;
    case EMIE_MOUSE_MOVED:
      if (button_pressed)
      {
        ic::vector3df rotation = node->getRotation();
        rotation.Y -= (event.MouseInput.X - old_x);
        old_x = event.MouseInput.X;
        old_y = event.MouseInput.Y;
        node->setRotation(rotation);
      }
      break;
    case EMIE_MOUSE_WHEEL:
      current_texture = (current_texture + 1) % textures.size();
      node->setMaterialTexture(0, textures[current_texture]);
      break;
    default:
      ;
    }*/

    return false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::gui_handler                                             *
\*------------------------------------------------------------------------*/
bool EventReceiver::gui_handler(const SEvent &event)
{
    if (!node) return false;
    switch(event.GUIEvent.EventType)
    {
    // Gestion des menus de la barre de menu
    case ig::EGET_MENU_ITEM_SELECTED:
    {
        ig::IGUIContextMenu *menu = (ig::IGUIContextMenu*)event.GUIEvent.Caller;
        s32 item = menu->getSelectedItem();
        s32 id = menu->getItemCommandId(item);
        u32 debug_info = node->isDebugDataVisible();

        switch(id)
        {
        case MENU_NEW_GAME:
            // Faire quelque chose ici !
            break;
        case MENU_QUIT:
            exit(0);

        case MENU_BOUNDING_BOX:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            node->setDebugDataVisible(debug_info ^ is::EDS_BBOX);
            break;

        case MENU_NORMALS:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            node->setDebugDataVisible(debug_info ^ is::EDS_NORMALS);
            break;

        case MENU_TRIANGLES:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            node->setDebugDataVisible(debug_info ^ is::EDS_MESH_WIRE_OVERLAY);
            break;

        case MENU_TRANSPARENCY:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            node->setDebugDataVisible(debug_info ^ is::EDS_HALF_TRANSPARENCY);
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

/**************************************************************************\
 * EventReceiver::OnEvent                                                 *
\**************************************************************************/
bool EventReceiver::OnEvent(const SEvent &event)
{
    if (!node) return false;

    switch (event.EventType)
    {
    case EET_KEY_INPUT_EVENT:
        //Release/FirstPress gestion
        KeyEvent[event.KeyInput.Key] = (KeyIsDown[event.KeyInput.Key] != event.KeyInput.PressedDown);
        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        break;
    case EET_MOUSE_INPUT_EVENT:
        return mouse_handler(event);
    case EET_GUI_EVENT:
        return gui_handler(event);
    default:;
    }

    return false;
}

/**************************************************************************\
 * EventReceiver::set_node                                                *
\**************************************************************************/
void EventReceiver::set_node(scene::IAnimatedMeshSceneNode *n)
{
    node = n;
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

