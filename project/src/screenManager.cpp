/*!
 * \file screenManager.cpp
 * \brief Implementation des fonctions de la classe ScreenManager.
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include "screenManager.h"

ScreenManager::ScreenManager(ig::IGUIEnvironment *_gui, ig::ICursorControl *_cursor,
                             iv::ITexture *_painTexture, float _screen_width, float _screen_height)
    :g(_gui), cursor(_cursor), painTexture(_painTexture), screen_width(_screen_width), screen_height(_screen_height)
{
    // La barre de menu
    menu = new Menu();
    menu->create_menu(g);
    // Une fenêtre pour différents réglages
    menu->create_window(g);

    // Image de prise de degats
    painImage = g->addImage(ic::rect<irr::s32>(0, 0, screen_width, screen_height));
    painImage->setScaleImage(true);
    painImage->setImage(painTexture);
    painImage->setColor(iv::SColor(50, 255, 0, 0));
    painImage->setVisible(false);

    // Création de la GUI
    // Choix de la police de caractères
    skin = g->getSkin();
    font = g->getFont("data/fontlucida.png");
    skin->setFont(font);
}

void ScreenManager::resize_screen(float width, float height)
{
    if(screen_width != width || screen_height != height)
    {
        screen_width = width;
        screen_height = height;

        painImage->drop();
        painImage = g->addImage(ic::rect<irr::s32>(0, 0, screen_width, screen_height));
        painImage->setScaleImage(true);
        painImage->setImage(painTexture);
        painImage->setColor(iv::SColor(50, 255, 0, 0));
        painImage->setVisible(false);
    }
}

void ScreenManager::displayPain(bool state)
{
    painImage->setVisible(state);
}

bool ScreenManager::isVisiblePain() const
{
    return painImage->isVisible();
}
